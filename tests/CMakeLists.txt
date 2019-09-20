add_executable(molecular-gfx-tests
	KtxTestData.cpp
	TgaTestData.cpp

	DdsTestData.cpp
	TestBox.cpp
	TestDdsFile.cpp
	TestFileTypeIdentification.cpp
	TestFrustum.cpp
	TestIniFile.cpp
	TestIteratorAdapters.cpp
	TestKtxFile.cpp

	TestPlane.cpp
	TestPlaneSet.cpp

	TestTgaFile.cpp
)

target_link_libraries(molecular-gfx-tests
	molecular::gfx
	molecular::testbed
)
