#pragma once

#include <map>

#include "ofXml.h"

#include "syntheffect/manager/Manager.h"

namespace syntheffect {
    namespace xml {
        class Parser {
            public:
                void parse(std::string path, std::shared_ptr<manager::Manager> m);

            private:
                void addPipelines(const ofXml& xml, std::shared_ptr<manager::Manager> m);
                void addPipeline(const ofXml& xml, std::shared_ptr<manager::Manager> m);
                void addShader(const ofXml& xml, std::shared_ptr<manager::Manager> m, int pipeline_id);
                void addShaderParam(const ofXml& xml, std::shared_ptr<manager::Manager> m, int pipeline_id, int effect_index);
        };
    }
}
