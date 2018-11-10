#ifndef SYNTHEFFECT_RACK_CAROUSEL
#define SYNTHEFFECT_RACK_CAROUSEL

#include <string>
#include <memory>
#include <vector>

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Carousel : public Module {
            public:
                Carousel(const std::string& id, std::vector<std::shared_ptr<Module>> modules);

                virtual std::shared_ptr<Channel> getOutput(const std::string& name) override;
                virtual void start() override;
                virtual void update(float t) override;
                virtual const std::string getType() override;

                static const std::string getModuleType();

            private:
                void updateOutputs();

                std::shared_ptr<Module> getCurrent();
                std::shared_ptr<Module> getNext();
                std::shared_ptr<Module> getPrev();

                std::vector<std::shared_ptr<Module>> modules_;
                float prev_trigger_value_ = 0;
                float next_trigger_value_ = 0;
        };
    }
}
#endif
