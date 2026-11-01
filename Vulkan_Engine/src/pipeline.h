#pragma once

#include "config.h"
#include "shaders.h"

namespace vkInit
{
    struct GraphicsPipeLineInBundle
    {
        vk::Device device;
        std::string vertexFilePath;
        std::string fragmentFilePath;
        vk::Extent2D swapchainExtent;
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

        // Vertex Shader 
        if (debug)
        {
            std::cout << "Create Vertex Shader Module \n";
        }
        vk::ShaderModule vertexShader = vkUtil::createModule(specification.vertexFilePath, specification.device, debug);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = {};
        vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderInfo.module = vertexShader;
        vertexShaderInfo.pName = "main";
        shaderStages.push_back(vertexShaderInfo);

        // ViewPort And Scissor
        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = specification.swapchainExtent.width;
        viewport.height = specification.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::Rect2D scissors = {};
        scissors.offset.x = 0.0f;
        scissors.offset.y = 0.0f;
        scissors.extent = specification.swapchainExtent;
        vk::PipelineViewportStateCreateInfo viewportState = {};
        viewportState.flags = vk::PipelineViewportStateCreateFlags();
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissors;
        pipelineInfo.pViewportState = &viewportState;

        // Rasterization
        vk::PipelineRasterizationStateCreateInfo rasterization = {};
        rasterization.flags = vk::PipelineRasterizationStateCreateFlags();
        rasterization.depthClampEnable = VK_FALSE;
        rasterization.rasterizerDiscardEnable = VK_FALSE;
        rasterization.polygonMode = vk::PolygonMode::eFill;
        rasterization.lineWidth = 1.0f;
        rasterization.cullMode = vk::CullModeFlagBits::eBack;
        rasterization.frontFace = vk::FrontFace::eClockwise;
        rasterization.depthBiasClamp = VK_FALSE;
        pipelineInfo.pRasterizationState = &rasterization;

        // Fragment Shader 
        if(debug)
        {
            std::cout << "Create Fragment Shader Module \n" ; 
        }
        vk::ShaderModule fragmentShader = vkUtil::createModule(specification.fragmentFilePath,specification.device,debug) ;
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {}; 
        fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags(); 
        fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderInfo.module = fragmentShader;
        fragmentShaderInfo.pName = "main";
        shaderStages.push_back(fragmentShaderInfo);
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();

        // Multi Sampling 
        vk::PipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
        multisampling.sampleShadingEnable = VK_FALSE; 
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        pipelineInfo.pMultisampleState = &multisampling;

        // color blending
        vk::PipelineColorBlendAttachmentState colorBlendAttachement = {};
        colorBlendAttachement.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA ; 
        colorBlendAttachement.blendEnable = VK_FALSE;
        vk::PipelineColorBlendStateCreateInfo colorBending = {};
        colorBending.flags = vk::PipelineColorBlendStateCreateFlags();
        colorBending.logicOpEnable = VK_FALSE;
        colorBending.logicOp = vk::LogicOp::eCopy;
        colorBending.attachmentCount = 1;
        colorBending.pAttachments = &colorBlendAttachement;
        colorBending.blendConstants[0] = 0.0f;
        colorBending.blendConstants[1] = 0.0f;
        colorBending.blendConstants[2] = 0.0f;
        colorBending.blendConstants[3] = 0.0f;
        pipelineInfo.pColorBlendState = &colorBending;



        GraphicsPipeLineOutBundle output = {};
        specification.device.destroyShaderModule(vertexShader);
        specification.device.destroyShaderModule(fragmentShader);
        return output;
    }
}