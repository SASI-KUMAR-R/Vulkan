#pragma once

#include "config.h"
#include "shaders.h"

namespace vkInit
{
    struct GraphicsPipeLineInBundle
    {
        vk::Device device;
        std::string vertexFilePath;
    };

    struct GraphicsPipeLineOutBundle
    {
    };

    GraphicsPipeLineOutBundle make_graphics_pipeline(GraphicsPipeLineInBundle specification, bool debug)
    {
        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.flags = vk::PipelineCreateFlags();

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        // vertex info
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        pipelineInfo.pVertexInputState = &vertexInputInfo;

        // Input Assembly
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
        inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
        inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

        // Vertex Assembly
        if (debug)
        {
            std::cout << "Create Vertex Shader Module \n";
        }
        vk::ShaderModule vertexShader = vkUtil::createModule(specification.vertexFilePath, specification.device, debug);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = {} ; 
        vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags() ; 
        vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex ; 
        vertexShaderInfo.module = vertexShader ; 
        vertexShaderInfo.pName = "main" ; 
        shaderStages.push_back(vertexShaderInfo) ; 


        GraphicsPipeLineOutBundle output = {};

        specification.device.destroyShaderModule(vertexShader) ; 
        return output;
    }
}