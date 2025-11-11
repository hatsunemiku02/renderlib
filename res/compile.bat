@echo off
set VULKAN_SDK=D:/workspace/renderlib/third/VulkanSDK/1.4.328.1/Bin
%VULKAN_SDK%/glslc.exe D:/workspace/renderlib/res/shader/vert.shader -o D:/workspace/renderlib/res/shader/vert.shader.spv
%VULKAN_SDK%/glslc.exe D:/workspace/renderlib/res/shader/frag.shader -o D:/workspace/renderlib/res/shader/frag.shader.spv
pause
D:/workspace/renderlib/third/VulkanSDK/1.4.328.1/Bin/glslc.exe D:/workspace/renderlib/res/shader/shader.vert -o D:/workspace/renderlib/res/shader/shader.vert.spv
D:/workspace/renderlib/third/VulkanSDK/1.4.328.1/Bin/glslc.exe D:/workspace/renderlib/res/shader/shader.frag -o D:/workspace/renderlib/res/shader/shader.frag.spv