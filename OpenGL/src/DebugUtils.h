#pragma once

#include <iostream>
#include "glad/glad.h"

namespace DebugUtils
{
    void APIENTRY OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        char* _severity{ nullptr };
        char* _source{ nullptr };
        char* _type{ nullptr };
        char* _message{ (char*)message };
        bool set_debug_break{ true };

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = (char*)"HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            return;
            _severity = (char*)"MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = (char*)"LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return;
            _severity = (char*)"NOTIFICATION";
            set_debug_break = false;
            break;

        default:
            _severity = (char*)"UNKNOWN";
            break;
        }


        switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = (char*)"API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = (char*)"WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = (char*)"SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = (char*)"THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = (char*)"APPLICATION";
            break;

        default:
            _source = (char*)"UNKNOWN";
            break;
        }

        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = (char*)"ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = (char*)"DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = (char*)"UDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = (char*)"PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = (char*)"PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = (char*)"OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = (char*)"MARKER";
            break;

        default:
            _type = (char*)"UNKNOWN";
            break;
        }

        std::cerr << "OpenGL Error!\n";
        std::cerr << "SEVERITY: " << _severity << '\n';
        std::cerr << "SOURCE: " << _source << '\n';
        std::cerr << "TYPE: " << _type << "\n";
        std::cerr << "MESSAGE: " << _message << "\n\n";

        if (set_debug_break)
        {
            __debugbreak();
        }
    }
}