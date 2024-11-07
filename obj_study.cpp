// Include the "sb7.h" header file
#include "sb7.h"

// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    GLuint compile_shaders(void)
    {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;

        static const GLchar* vertex_shader_source[] =
        {
            "#version 420 core                                                 \n"
            "                                                                  \n"
            "layout(location = 0) in vec4 offset;"
            "                                                                  \n"
            "void main(void)                                                   \n"
            "{                                                                 \n"
            "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
            "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
            "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
            "                                                                  \n"
            // gl_VertexID : ���������� ���� id (��������)
            "    gl_Position = vertices[gl_VertexID] + offset;                          \n"
            "}                                                                 \n"
        };

        // Source code for fragment shader
        static const GLchar* fragment_shader_source[] =
        {
            "#version 450 core                                  \n"
            "                                                   \n"
            "out vec4 color;                                    \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "    color = vec4(0.0, 0.8, 1.0, 1.0);              \n"
            "}                                                  \n"
        };

        // glCreateShader : empty shader ����
        // vertex shader ����
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        // glShaderSource : shader object �� shader source code �� �Ѱ��ش�.
        // �̶�, ���纻�� �Ѿ�� �ȴ�.
        glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
        
        // vertex shader compile
        // shader object �ȿ� �ִ� shader source code �� �߰� binary ���·� �������ش�.
        glCompileShader(vertex_shader);

        // create fragment shader
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
       
        // fragment shader compile
        glCompileShader(fragment_shader);

        // shader ���� link �Ͽ� program ���� link ��Ų��.
        // �̸� ���� opengl �� ������ �� �ְ� �ȴ�.
        //glCreateProgram : shader object ���� attach �� �� �ִ� program object ����
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        // shader object ���� shader processor �� ������ �� �ִ� ���¿� link �Ѵ�.
        glLinkProgram(program);

        // Delete the shaders as the program has them now
        // glDeleteShader : shader �� program object �� link �ǰ� ���� binary code ���·� ��
        //      ���� ���� ���̻� shader �� �ʿ����. 
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }
    void startup()
    {
        rendering_program = compile_shaders();

        /*
        * opengl ������ ���𰡸� �׸����� �� �� VAO �� �������Ѵ�.
        * vertex shader ���� input �� �����ϴ� ���̴�.
        * VAO �� opengl pipeline �� ���޵Ǵ� input �� ���õ� ��� state �� ������ �ִ�.
        */
        glCreateVertexArrays(1, &vertex_array_object);
        
        // ������ vao �� context �� attach�Ѵ�.
        glBindVertexArray(vertex_array_object);
    }

    void shutdown()
    {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vertex_array_object);
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
        glClearBufferfv(GL_COLOR, 0, color);

        // �츮�� ������ shader object �� �̿��Ͽ� rendering �ض�
        glUseProgram(rendering_program);

        GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
                         (float)cos(currentTime) * 0.6f,
                         0.0f, 0.0f };
        // Update the value of input attribute 0
        glVertexAttrib4fv(0, attrib);


        // 1st : � ������ primitive �� �׸� ���ΰ�
        // GL_POINTS : point �� �׸����� �Ѵ�.
        // 2nd : �ϴ� 0 ���� ���� (���߿� ����)
        // 3rd : �׸����� �ϴ� ������ ����
        //      �� ������ single vertex �� �νĵȴ�.
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
private:
    // �Ѱܹ��� shader program object �� �����Ѵ�.
    // �̸� ����, �ش� shader �� ����� �� �Ź� �ٽ� recompile �� �ʿ䰡 ����.
    GLuint rendering_program; 
    GLuint vertex_array_object;
};

// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);