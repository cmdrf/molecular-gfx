add_executable(molecular-example-humanhead
	HumanHeadExampleMain.cpp
)

target_link_libraries(molecular-example-humanhead
	molecular::gfx
)
