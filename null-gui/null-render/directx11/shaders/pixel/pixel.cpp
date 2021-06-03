#include "../shaders.h"

namespace null_render {
    namespace shaders {
        namespace pixel {
            void create_shader_control() {
                if (!shader) shader.init(shader_sources::pixel, sizeof(shader_sources::pixel));
            }

            void clear_shader_control() {
                if (shader) shader.clear();
            }
        }
    }
}