add_executable(molecular-gfx-tests
	DdsTestData.cpp
	KtxTestData.cpp
	TgaTestData.cpp

	TestBox.cpp
	TestDdsFile.cpp
	TestFileTypeIdentification.cpp
	TestFrustum.cpp
	TestIniFile.cpp
	TestIteratorAdapters.cpp
	TestKtxFile.cpp
	TestPlane.cpp
	TestPlaneSet.cpp
	TestQuaternion.cpp
	TestStringStore.cpp
	TestTgaFile.cpp
)

target_link_libraries(molecular-gfx-tests
	molecular::gfx
	molecular::testbed
)
