#include "../shaders.h"

namespace null {
    namespace render {
        namespace shaders {
            namespace vertex {
                void create_shader_control() {
                    if(!shader) shader.init(sources::vertex, sizeof(sources::vertex), sizeof(constant));
                }

                void clear_shader_control() {
                    if(shader) shader.clear();
                }
            }
        }
    }
}