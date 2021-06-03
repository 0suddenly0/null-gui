#include "../shaders.h"

namespace null_render {
	namespace shaders {
		namespace vertex {
            void create_shader_control() {
                if (!shader) shader.init(shader_sources::vertex);
            }

            void clear_shader_control() {
                if (shader) shader.clear();
            }
		}
	}
}