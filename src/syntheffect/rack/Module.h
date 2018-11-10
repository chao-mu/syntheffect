#ifndef SYNTHEFFECT_RACK_MODULE
#define SYNTHEFFECT_RACK_MODULE

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <memory>

#include "syntheffect/rack/Channel.h"

namespace syntheffect {
    namespace rack {
        class Module {
            public:
                Module(const std::string& id);
                virtual void setup(int width, int height, int internal_format);
                virtual void start();
                virtual bool isReady();
                virtual void update(float t) = 0;
                virtual const std::string getType() = 0;

                void setInput(const std::string& name, std::shared_ptr<Channel> channel);

                virtual std::shared_ptr<Channel> getOutput(const std::string& name);

                const std::string id_;

            protected:
                float getInputConstant(const std::string& name, float def);

                std::map<std::string, std::shared_ptr<Channel>> output_channels_;
                std::map<std::string, std::shared_ptr<Channel>> input_channels_;
        };
    }
}

#endif
