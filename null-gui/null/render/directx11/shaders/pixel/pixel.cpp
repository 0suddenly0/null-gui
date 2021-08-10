#include "../shaders.h"

namespace null {
    namespace render {
        namespace shaders {
            namespace pixel {
                void create_shader_control() {
                    if(!shader) shader.init(sources::pixel, sizeof(sources::pixel));
                }

                void clear_shader_control() {
                    if(shader) shader.clear();
                }
            }
        }
    }
}