module;


#include <bgfx/bgfx.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>


export module shader;

export class Shader {


	bgfx::ShaderHandle vertex_handle;
	bgfx::ShaderHandle fragment_handle;

	bgfx::ShaderHandle loadShader(const char* _name) {
		std::ifstream file(_name, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			std::cerr << "Failed to open shader file: " << _name << std::endl;
			return BGFX_INVALID_HANDLE;
		}

		std::streamsize fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> data(fileSize);
		if (!file.read(data.data(), fileSize)) {
			std::cerr << "Failed to read shader file: " << _name << std::endl;
			return BGFX_INVALID_HANDLE;
		}

		const bgfx::Memory* mem = bgfx::copy(data.data(), static_cast<uint32_t>(fileSize));
		bgfx::ShaderHandle handle = bgfx::createShader(mem);
		bgfx::setName(handle, _name);

		return handle;
	}

public:
	bgfx::ProgramHandle program;

	bgfx::ProgramHandle get_program() const {
		return program;
	}

	void load(const char* vertex_path, const char* fragment_path) {
		vertex_handle = loadShader(vertex_path);
		fragment_handle = loadShader(fragment_path);

		if (bgfx::isValid(vertex_handle) && bgfx::isValid(fragment_handle)) {
			program = bgfx::createProgram(vertex_handle, fragment_handle, true);
		}
		else {
			std::cerr << "Failed to load shader program" << std::endl;
		}
	}

	~Shader() {
		bgfx::destroy(vertex_handle);
		bgfx::destroy(fragment_handle);
		bgfx::destroy(program);
	}
};