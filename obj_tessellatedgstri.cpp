// Include the "sb7.h" header file
#include "sb7.h"

// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    GLuint compile_shaders(void)
    {
        GLuint program;

        static const char* vs_source[] =
        {
            "#version 410 core                                                 \n"
            "                                                                  \n"
            "void main(void)                                                   \n"
            "{                                                                 \n"
            "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
            "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
            "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
            "                                                                  \n"
            "    gl_Position = vertices[gl_VertexID];                          \n"
            "}                                                                 \n"
        };

        static const char* tcs_source[] =
        {
            "#version 410 core                                                                 \n"
            "                                                                                  \n"
            "layout (vertices = 3) out;                                                        \n"
            "                                                                                  \n"
            "void main(void)                                                                   \n"
            "{                                                                                 \n"
            "    if (gl_InvocationID == 0)                                                     \n"
            "    {                                                                             \n"
            "        gl_TessLevelInner[0] = 5.0;                                               \n"
            "        gl_TessLevelOuter[0] = 2.0;                                               \n"
            "        gl_TessLevelOuter[1] = 2.0;                                               \n"
            "        gl_TessLevelOuter[2] = 2.0;                                               \n"
            "    }                                                                             \n"
            "    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     \n"
            "}                                                                                 \n"
        };

        static const char* tes_source[] =
        {
            "#version 410 core                                                                 \n"
            "                                                                                  \n"
            "layout (triangles, equal_spacing, cw) in;                                         \n"
            "                                                                                  \n"
            "void main(void)                                                                   \n"
            "{                                                                                 \n"
            "    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
            "                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
            "                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
            "}                                                                                 \n"
        };

        static const char* gs_source[] =
        {
            "#version 410 core                                                                  \n"
            "                                                                                   \n"
            "layout (triangles) in;                                                             \n"
            "layout (points, max_vertices = 3) out;                                             \n"
            "                                                                                   \n"
            "void main(void)                                                                    \n"
            "{                                                                                  \n"
            "    int i;                                                                         \n"
            "                                                                                   \n"
            "    for (i = 0; i < gl_in.length(); i++)                                           \n"
            "    {                                                                              \n"
            "        gl_Position = gl_in[i].gl_Position;                                        \n"
            "        EmitVertex();                                                              \n"
            "    }                                                                              \n"
            "}                                                                                  \n"
        };

        static const char* fs_source[] =
        {
            "#version 410 core                                                 \n"
            "                                                                  \n"
            "out vec4 color;                                                   \n"
            "                                                                  \n"
            "void main(void)                                                   \n"
            "{                                                                 \n"
            "    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
            "}                                                                 \n"
        };

        GLuint vs;
        GLuint tcs;
        GLuint tes;
        GLuint gs;
        GLuint fs;

        {
            // glCreateShader : empty shader ����
            // vertex shader ����
            vs = glCreateShader(GL_VERTEX_SHADER);
            // glShaderSource : shader object �� shader source code �� �Ѱ��ش�.
            // �̶�, ���纻�� �Ѿ�� �ȴ�.
            glShaderSource(vs, 1, vs_source, NULL);
            // vertex shader compile
            // shader object �ȿ� �ִ� shader source code �� 
            // �߰� binary ���·� �������ش�.
            glCompileShader(vs);
        }

        {
            tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tcs, 1, tcs_source, NULL);
            glCompileShader(tcs);
        }

        {
            tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tes, 1, tes_source, NULL);
            glCompileShader(tes);
        }

        {
            gs = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gs, 1, gs_source, NULL);
            glCompileShader(gs);
        }

        {
            // create fragment shader
            fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, fs_source, NULL);
            // fragment shader compile
            glCompileShader(fs);
        }

        // shader ���� link �Ͽ� program ���� link ��Ų��.
        // �̸� ���� opengl �� ������ �� �ְ� �ȴ�.
        //glCreateProgram : shader object ���� attach �� �� �ִ� program object ����
        program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, tcs);
        glAttachShader(program, tes);
        glAttachShader(program, gs);
        glAttachShader(program, fs);

        // shader object ���� shader processor �� ������ �� �ִ� ���¿� link �Ѵ�.
        glLinkProgram(program);

        // Delete the shaders as the program has them now
        // glDeleteShader : shader �� program object �� link �ǰ� ���� binary code ���·� ��
        //      ���� ���� ���̻� shader �� �ʿ����. 
        // glDeleteShader(vs);
        // glDeleteShader(tcs);
        // glDeleteShader(tes);
        // glDeleteShader(gs);
        // glDeleteShader(fs);

        return program;
    }
    void startup()
    {
        rendering_program = compile_shaders();

        /*
        * opengl ������ ���𰡸� �׸����� �� �� VAO �� �����Ѵ�.
        * vertex shader ���� input �� �����ϴ� ���̴�.
        * VAO �� opengl pipeline �� ���޵Ǵ� input �� ���õ� ��� state �� ������ �ִ�.
        */
        glGenVertexArrays(1, &vertex_array_object);

        // ������ vao �� context �� attach�Ѵ�.
        glBindVertexArray(vertex_array_object);

        // tesselation ����� ���� ���ؼ� opegnl ����
        // ��� trinagle ���� outline �� ���� �ʹٶ�� ����ϴ� ��
        // 1st : face
        // - � type �� polygon �� ������ �ְ� ������
        // 2nd : mode
        // - polygon �� ��� �׷����⸦ ���ϴ� ��
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void shutdown()
    {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
    }

    void render(double currentTime)
    {
        /*
        // Simply clear the window with red
        // 4 th : alpha
        const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
                              (float)cos(currentTime) * 0.5f + 0.5f,
                              0.0f, 0.3f };
        //
        // glClearBufferfv : 1��°, 3��° ���� �������� buffer �� clear �϶�
        // 2nd : multiple output buffer �� ���� ���
        //
        glClearBufferfv(GL_COLOR, 0, color);
        */

        /* >> Single Point �׸���
        *  const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
                              (float)cos(currentTime) * 0.5f + 0.5f,
                              0.0f, 1.0f };

        glClearBufferfv(GL_COLOR, 0, color);

        // �츮�� ������ shader object �� �̿��Ͽ� rendering �ض�
        glUseProgram(rendering_program);

        // point �� ũ�⸦ ����
        // max �� �����Ǵ� point size �� 64 pixel �̴�.
        glPointSize(40.0f);

        // 1st : � ������ primitive �� �׸� ���ΰ�
        // GL_POINTS : point �� �׸����� �Ѵ�.
        // 2nd : �ϴ� 0 ���� ���� (���߿� ����)
        // 3rd : �׸����� �ϴ� ������ ����
        //      �� ������ single vertex �� �νĵȴ�.
        glDrawArrays(GL_POINTS, 0, 1);
        */

        /* >> �ﰢ�� �׸���
        */
        const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
        const GLfloat colorLayout1[] = { 1.0f, 0.2f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        // �츮�� ������ shader object �� �̿��Ͽ� rendering �ض�
        glUseProgram(rendering_program);

        GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
                         (float)cos(currentTime) * 0.6f,
                         0.0f, 0.0f };
        // Update the value of input attribute 0
        glVertexAttrib4fv(0, attrib);
        glVertexAttrib4fv(1, colorLayout1);

        // ���� ����� 5.f �� �Ѵ�.
        glPointSize(5.0f);

        // 1st : � ������ primitive �� �׸� ���ΰ�
        // GL_PATCHES : patch �� �׸����� �Ѵ�.
        // 2nd : �ϴ� 0 ���� ���� (���߿� ����)
        // 3rd : �׸����� �ϴ� ������ ����
        //      3���� ������ ����Ͽ� �ﰢ�� ����� patch ����
        glDrawArrays(GL_PATCHES, 0, 3);
    }
private:
    // �Ѱܹ��� shader program object �� �����Ѵ�.
    // �̸� ����, �ش� shader �� ����� �� �Ź� �ٽ� recompile �� �ʿ䰡 ����.
    GLuint rendering_program;
    GLuint vertex_array_object;
};

// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);