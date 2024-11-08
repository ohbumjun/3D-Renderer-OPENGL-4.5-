//// Include the "sb7.h" header file
//#include "sb7.h"
//
//// Derive my_application from sb7::application
//class my_application : public sb7::application
//{
//public:
//    GLuint compile_shaders(void)
//    {
//        GLuint program;
//
//        static const char* vs_source[] =
//        {
//            "#version 410 core                                                 \n"
//            "                                                                  \n"
//            "void main(void)                                                   \n"
//            "{                                                                 \n"
//            "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
//            "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
//            "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
//            "                                                                  \n"
//            "    gl_Position = vertices[gl_VertexID];                          \n"
//            "}                                                                 \n"
//        };
//
//        static const char* tcs_source[] =
//        {
//            "#version 410 core                                                                 \n"
//            "                                                                                  \n"
//            "layout (vertices = 3) out;                                                        \n"
//            "                                                                                  \n"
//            "void main(void)                                                                   \n"
//            "{                                                                                 \n"
//            "    if (gl_InvocationID == 0)                                                     \n"
//            "    {                                                                             \n"
//            "        gl_TessLevelInner[0] = 5.0;                                               \n"
//            "        gl_TessLevelOuter[0] = 2.0;                                               \n"
//            "        gl_TessLevelOuter[1] = 2.0;                                               \n"
//            "        gl_TessLevelOuter[2] = 2.0;                                               \n"
//            "    }                                                                             \n"
//            "    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     \n"
//            "}                                                                                 \n"
//        };
//
//        static const char* tes_source[] =
//        {
//            "#version 410 core                                                                 \n"
//            "                                                                                  \n"
//            "layout (triangles, equal_spacing, cw) in;                                         \n"
//            "                                                                                  \n"
//            "void main(void)                                                                   \n"
//            "{                                                                                 \n"
//            "    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
//            "                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
//            "                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
//            "}                                                                                 \n"
//        };
//
//        static const char* fs_source[] =
//        {
//            "#version 410 core                                                 \n"
//            "                                                                  \n"
//            "out vec4 color;                                                   \n"
//            "                                                                  \n"
//            "void main(void)                                                   \n"
//            "{                                                                 \n"
//            "    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
//            "}                                                                 \n"
//        };
//
//        GLuint vs;
//        GLuint tcs;
//        GLuint tes;
//        GLuint fs;
//
//        {
//            // glCreateShader : empty shader 생성
//            // vertex shader 생성
//            vs = glCreateShader(GL_VERTEX_SHADER);
//            // glShaderSource : shader object 에 shader source code 를 넘겨준다.
//            // 이때, 복사본이 넘어가게 된다.
//            glShaderSource(vs, 1, vs_source, NULL);
//            // vertex shader compile
//            // shader object 안에 있는 shader source code 를 
//            // 중간 binary 형태로 변경해준다.
//            glCompileShader(vs);
//        }
//
//        {
//            tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
//            glShaderSource(tcs, 1, tcs_source, NULL);
//            glCompileShader(tcs);
//        }
//
//        {
//            tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
//            glShaderSource(tes, 1, tes_source, NULL);
//            glCompileShader(tes);
//        }
//
//        {
//            // create fragment shader
//            fs = glCreateShader(GL_FRAGMENT_SHADER);
//            glShaderSource(fs, 1, fs_source, NULL);
//            // fragment shader compile
//            glCompileShader(fs);
//        }
//        
//        // shader 들을 link 하여 program 으로 link 시킨다.
//        // 이를 통해 opengl 이 실행할 수 있게 된다.
//        //glCreateProgram : shader object 들을 attach 할 수 있는 program object 생성
//        program = glCreateProgram();
//
//        glAttachShader(program, vs);
//        glAttachShader(program, tcs);
//        glAttachShader(program, tes);
//        glAttachShader(program, fs);
//
//        // shader object 들을 shader processor 가 실행할 수 있는 형태오 link 한다.
//        glLinkProgram(program);
//
//        // Delete the shaders as the program has them now
//        // glDeleteShader : shader 가 program object 에 link 되고 나면 binary code 형태로 들어감
//        //      따라서 이제 더이상 shader 는 필요없다. 
//        // glDeleteShader(vs);
//        // glDeleteShader(tcs);
//        // glDeleteShader(tes);
//        // glDeleteShader(fs);
//
//        return program;
//    }
//    void startup()
//    {
//        rendering_program = compile_shaders();
//
//        /*
//        * opengl 에서는 무언가를 그리고자 할 때 VAO 를 생성한다.
//        * vertex shader 측에 input 을 공급하는 것이다.
//        * VAO 는 opengl pipeline 에 공급되는 input 과 관련된 모든 state 를 가지고 있다.
//        */
//        glGenVertexArrays(1, &vertex_array_object);
//        
//        // 생성한 vao 를 context 에 attach한다.
//        glBindVertexArray(vertex_array_object);
//
//        // tesselation 결과를 보기 위해서 opegnl 에게
//        // 결과 trinagle 들의 outline 만 보고 싶다라고 얘기하는 것
//        // 1st : face
//        // - 어떤 type 의 polygon 에 영향을 주고 싶은가
//        // 2nd : mode
//        // - polygon 이 어떻게 그려지기를 원하는 가
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    }
//
//    void shutdown()
//    {
//        glDeleteVertexArrays(1, &vertex_array_object);
//        glDeleteProgram(rendering_program);
//    }
//
//    void render(double currentTime)
//    {
//        /*
//        // Simply clear the window with red
//        // 4 th : alpha
//        const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
//                              (float)cos(currentTime) * 0.5f + 0.5f,
//                              0.0f, 0.3f };
//        // 
//        // glClearBufferfv : 1번째, 3번째 인자 색상으로 buffer 를 clear 하라
//        // 2nd : multiple output buffer 가 있을 경우
//        // 
//        glClearBufferfv(GL_COLOR, 0, color);
//        */
//
//        /* >> Single Point 그리기
//        *  const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
//                              (float)cos(currentTime) * 0.5f + 0.5f,
//                              0.0f, 1.0f };
//
//        glClearBufferfv(GL_COLOR, 0, color);
//
//        // 우리가 생성한 shader object 를 이용하여 rendering 해라
//        glUseProgram(rendering_program);
//
//        // point 의 크기를 조절
//        // max 로 지원되는 point size 는 64 pixel 이다.
//        glPointSize(40.0f);
//
//        // 1st : 어떤 유형의 primitive 를 그릴 것인가
//        // GL_POINTS : point 를 그리고자 한다.
//        // 2nd : 일단 0 으로 세팅 (나중에 설명)
//        // 3rd : 그리고자 하는 정점의 개수
//        //      각 정점은 single vertex 로 인식된다.
//        glDrawArrays(GL_POINTS, 0, 1);
//        */
//
//        /* >> 삼각형 그리기
//        */
//        const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
//        const GLfloat colorLayout1[] = { 1.0f, 0.2f, 0.0f, 1.0f };
//        glClearBufferfv(GL_COLOR, 0, color);
//
//        // 우리가 생성한 shader object 를 이용하여 rendering 해라
//        glUseProgram(rendering_program);
//
//        GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
//                         (float)cos(currentTime) * 0.6f,
//                         0.0f, 0.0f };
//        // Update the value of input attribute 0
//        glVertexAttrib4fv(0, attrib);
//        glVertexAttrib4fv(1, colorLayout1);
//
//        // 1st : 어떤 유형의 primitive 를 그릴 것인가
//        // GL_PATCHES : patch 를 그리고자 한다.
//        // 2nd : 일단 0 으로 세팅 (나중에 설명)
//        // 3rd : 그리고자 하는 정점의 개수
//        //      3개의 정점을 사용하여 삼각형 모양의 patch 형성
//        glDrawArrays(GL_PATCHES, 0, 3);
//    }
//private:
//    // 넘겨받은 shader program object 를 저장한다.
//    // 이를 통해, 해당 shader 를 사용할 때 매번 다시 recompile 할 필요가 없다.
//    GLuint rendering_program; 
//    GLuint vertex_array_object;
//};
//
//// Our one and only instance of DECLARE_MAIN
//DECLARE_MAIN(my_application);