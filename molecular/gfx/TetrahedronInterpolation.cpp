/*	TetrahedronInterpolation.cpp
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "TetrahedronInterpolation.h"
#include <cassert>
#include <stdexcept>
#include <molecular/util/Logging.h>
#include <molecular/util/MeshUtils.h>
#include <molecular/util/Math.h>

namespace molecular
{
namespace gfx
{

const Matrix<3, 9> TetrahedronInterpolation::kZeros = Matrix<3, 9>::Zeros();

void TetrahedronInterpolation::SetFileData(Blob&& fileData)
{
	mFileData = std::move(fileData);
	assert(mFileData.GetData());
	const TetrahedronSpaceFile* file = static_cast<const TetrahedronSpaceFile*>(mFileData.GetData());
	if(file->magic != TetrahedronSpaceFile::kMagic)
		throw std::runtime_error("Not a tetrahedron space file");
	if(file->version != TetrahedronSpaceFile::kVersion)
		throw std::runtime_error("Wrong tetrahedron space file version");
}

Matrix<3, 9> TetrahedronInterpolation::GetShCoefficients(const Vector3& position, int& tetIndex) const
{
	if(!mFileData.GetData())
		return kZeros;
	Vector4 weights = GetLightProbeInterpolationWeights(position, tetIndex);
	const TetrahedronSpaceFile* file = static_cast<const TetrahedronSpaceFile*>(mFileData.GetData());
	if (tetIndex < 0 || tetIndex >= file->numTetrahedrons)
		return kZeros;
	auto& tet = file->tetrahedra[tetIndex];
	auto verts = file->GetVertices();
	Matrix<3, 9> output = kZeros;
	for(int i = 0; i < 3; ++i)
		output += verts[tet.vertices[i]].coefficients * weights[i];
	if(tet.vertices[3] >= 0)
		output += verts[tet.vertices[3]].coefficients * weights[3];
	return output;
}

void TetrahedronInterpolation::GetTetrahedronCorners(int tetIndex, Vector3 outCorners[4], bool& outIsOuterCell) const
{
	if(!mFileData.GetData())
	{
		for(int i = 0; i < 4; i++)
			outCorners[i] = Vector3(0, 0, 0);
		outIsOuterCell = false;
		return;
	}

	const TetrahedronSpaceFile* file = static_cast<const TetrahedronSpaceFile*>(mFileData.GetData());
	if(tetIndex >= 0 && tetIndex < file->numTetrahedrons)
	{
		auto& tet = file->tetrahedra[tetIndex];
		auto verts = file->GetVertices();
		for(int i = 0; i < 3; ++i)
			outCorners[i] = verts[tet.vertices[i]].position;

		if(tet.vertices[3] == -1)
			outIsOuterCell = true;
		else
		{
			outCorners[3] = verts[tet.vertices[3]].position;
			outIsOuterCell = false;
		}
	}
}

Vector4 TetrahedronInterpolation::GetCornerWeights(int tetIndex, const Vector3& position) const
{
	if(!mFileData.GetData())
		return Vector4(0, 0, 0, 0);

	const TetrahedronSpaceFile* file = static_cast<const TetrahedronSpaceFile*>(mFileData.GetData());
	const TetrahedronSpaceFile::Tetrahedron& tet = file->tetrahedra[tetIndex];
	return GetBarycentricCoordinates(position, tet);
}

Vector4 TetrahedronInterpolation::GetLightProbeInterpolationWeights(const Vector3& position, int& tetIndex) const
{
	assert(mFileData.GetData());
	const TetrahedronSpaceFile* file = static_cast<const TetrahedronSpaceFile*>(mFileData.GetData());

	// If we don't have an initial guess, always start from tetrahedron 0.
	// Tetrahedron 0 is picked to be roughly in the center of the probe cloud,
	// to minimize the number of steps to any other tetrahedron.
	const int tetCount = file->numTetrahedrons;
	if (tetIndex < 0 || tetIndex >= tetCount)
		tetIndex = 0;

	Vector4 weights(0, 0, 0, 0);
	for(int steps = 0; steps < tetCount; steps++)
	{
		const TetrahedronSpaceFile::Tetrahedron& tet = file->tetrahedra[tetIndex];
		weights = GetBarycentricCoordinates(position, tet);
		if(weights[0] >= 0.0f && weights[1] >= 0.0f && weights[2] >= 0.0f && weights[3] >= 0.0f)
		{
			//Success!
			return weights;
		}
		// Otherwise find the smallest barycentric coord and move in that direction
		if (weights[0] < weights[1] && weights[0] < weights[2] && weights[0] < weights[3])
			tetIndex = tet.neighbours[0];
		else if (weights[1] < weights[2] && weights[1] < weights[3])
			tetIndex = tet.neighbours[1];
		else if (weights[2] < weights[3])
			tetIndex = tet.neighbours[2];
		else
			tetIndex = tet.neighbours[3];
	}
	return weights;
}


Vector4 TetrahedronInterpolation::GetBarycentricCoordinates(const Vector3& p, const TetrahedronSpaceFile::Tetrahedron& tet) const
{
	const TetrahedronSpaceFile* file = static_cast<const TetrahedronSpaceFile*>(mFileData.GetData());
	auto vertices = file->GetVertices();
	if(tet.vertices[3] >= 0)
	{
		// Inner cell:
		auto vertex3 = vertices[tet.vertices[3]].position;
		Vector3 mult = Matrix3(tet.matrix) * (p - vertex3);
		float w = 1.0f - mult[0] - mult[1] - mult[2];
		assert(!std::isnan(w));
		return Vector4(mult, w);
	}
	else // Outer cell:
	{
		const int16_t (&ind)[4] = tet.vertices;
		const Vector3& v0 = vertices[ind[0]].position;
		const Vector3& v1 = vertices[ind[1]].position;
		const Vector3& v2 = vertices[ind[2]].position;
		float t = (p - v0).DotProduct(MeshUtils::TriangleNormal(v0, v1, v2));
		if(t < 0)
		{
			// p is below the hull surface of this tetrahedron, so let's just return the 4th barycentric coordinate
			// as the lowest (and negative), so that the tetrahedron adjacent at the base gets tested next
			return Vector4(0, 0, 0, -1);
		}
		// CalculateOuterTetrahedraMatrices() prepares the Tetrahedron.matrix, so that
		// the coefficients of the cubic can be found just like that:
		Eigen::Vector3f polyCoeffs = (tet.matrix.ToEigen() * Eigen::Vector4f(p[0], p[1], p[2], 1.0f)).head<3>();
		// If the polynomial degenerated to quadratic, the unused ind[3] will be set to -2 instead of -1
		if(ind[3] == -1)
			t = Math::PositiveCubicPolynomialRoot(polyCoeffs[0], polyCoeffs[1], polyCoeffs[2]);
		else
			t = Math::PositiveQuadraticPolynomialRoot(polyCoeffs[0], polyCoeffs[1], polyCoeffs[2]);
		// We could directly calculate the barycentric coords by plugging t into a*(A + t*Ap) + b*(B + t*Bp) = C + t*Cp, checking which coord to ignore
		// and using the two other equations, but it's actually almost the same as using BarycentricCoordinates3DTriangle()
		auto normals = file->GetNormals();

		Vector3 tri0 = v0 + normals[ind[0]] * t;
		Vector3 tri1 = v1 + normals[ind[1]] * t;
		Vector3 tri2 = v2 + normals[ind[2]] * t;
		return Vector4(Math::Barycentric(tri0, tri1, tri2, p), 0);
	}
}

}
}
