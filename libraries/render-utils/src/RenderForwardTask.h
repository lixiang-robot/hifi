//
//  RenderForwardTask.h
//  render-utils/src/
//
//  Created by Zach Pomerantz on 12/13/2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_RenderForwardTask_h
#define hifi_RenderForwardTask_h

#include <gpu/Pipeline.h>
#include <render/RenderFetchCullSortTask.h>
#include "LightingModel.h"

class RenderForwardTask : public render::Task {
public:
    using JobModel = Model<RenderForwardTask>;

    RenderForwardTask(RenderFetchCullSortTask::Output items);
};

class PrepareFramebuffer {
public:
    using Inputs = gpu::FramebufferPointer;
    using JobModel = render::Job::ModelO<PrepareFramebuffer, Inputs>;

    void run(const render::SceneContextPointer& sceneContext, const render::RenderContextPointer& renderContext,
            gpu::FramebufferPointer& framebuffer);

private:
    gpu::FramebufferPointer _framebuffer;
};

class Draw {
public:
    using Inputs = render::VaryingSet2 <render::ItemBounds, LightingModelPointer>;
    using JobModel = render::Job::ModelI<Draw, Inputs>;

    Draw(const render::ShapePlumberPointer& shapePlumber) : _shapePlumber(shapePlumber) {}
    void run(const render::SceneContextPointer& sceneContext, const render::RenderContextPointer& renderContext,
            const Inputs& inputs);

private:
    render::ShapePlumberPointer _shapePlumber;
};

class DrawTransparentDeferred {
public:
    using Inputs = render::VaryingSet2 <render::ItemBounds, LightingModelPointer>;
    //using Config = DrawConfig;
    using JobModel = render::Job::ModelI<DrawTransparentDeferred, Inputs/*, Config*/>;
    DrawTransparentDeferred(render::ShapePlumberPointer shapePlumber) : _shapePlumber{ shapePlumber } {}
    //void configure(const Config& config) { _maxDrawn = config.maxDrawn; }
    void run(const render::SceneContextPointer& sceneContext, const render::RenderContextPointer& renderContext, const Inputs& inputs);
protected:
    render::ShapePlumberPointer _shapePlumber;
    //int _maxDrawn; // initialized by Config
};

class Stencil {
public:
    using JobModel = render::Job::Model<Stencil>;

    void run(const render::SceneContextPointer& sceneContext, const render::RenderContextPointer& renderContext);

private:
    const gpu::PipelinePointer getPipeline();
    gpu::PipelinePointer _stencilPipeline;
};

class DrawBackground {
public:
    using Inputs = render::ItemBounds;
    using JobModel = render::Job::ModelI<DrawBackground, Inputs>;

    void run(const render::SceneContextPointer& sceneContext, const render::RenderContextPointer& renderContext,
            const Inputs& background);
};

#endif // hifi_RenderForwardTask_h
