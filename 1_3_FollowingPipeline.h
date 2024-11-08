// Vertex Shader

#pragma region Vertex Shader

/*
Vertex Shader 실행 전에 Vertex fetcing 이라는 단계 존재
여기서 vertex shader 측에 자동으로 input 을 넘겨준다.

그리고 variable ? 은 vertex attribute 라고 한다.
vertex attribute 은 open gl 에서 vertex data 가 어떻게
구성되는지를 정의한다.

vertex attribute 를 정의하려면 vertex shader 에서 'in' 을 사용한다.

// 'offset' is an input vertex attribute
#version 450 core

// 'offset' is an input vertex attribute
layout(location = 0) in vec4 offset;

void main(void)
{
    const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
        vec4(-0.25, -0.25, 0.5, 1.0),
        vec4(0.25, 0.25, 0.5, 1.0));

    // Add 'offset' to our hard-coded vertex position
    gl_Position = vertices[gl_VertexID] + offset;
}

위를 보면 offset 이라는 변수를 vertex  shader 에 input 으로 넣어주었다.
해당 값은 vertex fetch stage 에서 자동으로 채워준다.

해당 variable, 즉, vertex shader input 에 어떤 값이 들어갈지를
채워주는 많은 함수들 중에서
glVertexAttrib4fv() 가 있다.

// index : reference to the vertex attribute
// v : pointer to the data that will be used to fill the vertex attribute
// 위에 location = 0 에서 '0' 이 index 에 해당한다.
// 이때 이러한 것을 'layout qualifier' 라고 부른다.
void glVertexAttrib4fv(GLuint index,
    const GLfloat* v)

    ----------------------------------------------------------------------------

    in, out 변수를 통해 shader 간에 변수를 서로 pass 해줄 수도 있다.

    vertex shader 에서 'out' variable 을 사용하면
    해당 값을 fragment shader 로 'in' 변수를 통해 넘겨줄 수 있다.

    ----------------------------------------------------------------------------
    그런데 보통 하나의 구조체 형태로 out, in variable 을
    구성해줄 수도 있다.

    이때 여러 형태의 varaible 을 'interface block' 이라는 개념으로
    묶을 수 있다.

    이는 마치 struct 선언과 비슷하다.
    자.interface block 은 shader 와 shader 간에 데이터를
    이동시킬 때 사용하는 개념이다.

    vertex shader 로의 input 혹은
    fragment shader 로부터의 output 들을
    하나로 묶는데에는 사용할 수 없다.

    // Declare VS_OUT as an output interface block
    out VS_OUT
{
    vec4 color;     // Send color to the next stage
} vs_out;

void main(void)
{
    const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
        vec4(-0.25, -0.25, 0.5, 1.0),
        vec4(0.25, 0.25, 0.5, 1.0));

    // Add 'offset' to our hard-coded vertex position
    gl_Position = vertices[gl_VertexID] + offset;

    // Output a fixed value for vs_color
    vs_out.color = color;
}
}
*/
#pragma endregion

#pragma region Tessellation (TCS)

/*
- opengl 에서 tessellation 은 3가지 단계로 이루어진다.

{
    1) Tessellation Control Shader (TCS)
    - vertex shader 로부터 input 을 받고 2가지 일을 한다.
    a. tesesslation level 을 결정한다.
    b. tesselation evaluation shader 로 넘겨줄 data 를 생성한다.

    2) Fixed Function Tessellation Engine

    3) Tessellation Evaluation Shader
}

- opengl 에서 tesellation 은 일명 'patches' 라고 불리는
high-order surface 들을 point, line, traingle 로 break down 한다.

각 patch 는 'control point' 라고 하는 대상들로 구성된다.

각 patch 별 control pont 의 개수는 정할 수 있고 이는
glPatchParameteri() 를 통해 설정할 수 있다.

보통 value 인자에 아무것도 안 넣어주면 default 3

최대 32 까지만 설정하는 것이 권장된다.

// ex) 
void glPatchParameteri(GLenum pname,
                       GLint value); // valud : number of control points


Tesselation 이 활성화되면
vertex shader 는 control point 마다 실행된다.

한편 TCS 는 control ponit 의 group 별로 실행된다. batch 라는
단위 별로 실행 ?

이때 batch 의 size 은 patch 별 control point 의 개수이다.

즉, 정점들은 control point 처럼 사용되고
vertex shader 의 결과물은, batches 로 들어가게 된다.
그리고 이 batche 는 TCS 의 input 이 된다.

이후 TCS 단계를 거쳐서
patch 별 control point 의 개수는 변할 수 있다.
TCS 에 의해서 control point 가 생성될 수 도 있기 때문이다.


아래 함수를 통해서 TCS 에 의해 생성되는 control point 개수를
지정할 수 있다.
layout (vertices = N) out;

이때 N 은 patch 별 control point 의 개수이다.

control shader 는 결과 control point 값을 계산하고
결과물 patch 에 대한 tesellation factor 를 지정한다.

그리고 이는 fixed function tessellation 단계로 넘어간다.
해당 단계로 넘어가는 output variable 은
opengl built-in variable 로 정해져 있다.

gl_TessLevelInner 
gl_TessLevelOuter
2개 변수는 테셀레이션 수준을 결정하는 인자
테셀레이션 모드에 따라 어느 값까지 쓰이는지가 달라진다.
이 모드는 TES에서 input layout qualifier를 이용하여 설정되며 
quads, triangles, isolines 중 하나

모드가 quads일 경우 모든 값인다.
모드가 triangles일 경우 inner는 0, outer는 1,2,3만 쓰인다.
모드가 isolines일 경우 outer 0,1만 쓰인다.

혹은 user defined output variable 로 넘겨줄 수도 있다.
ex) gl_out array

----------------------------------------------------------------------------

#version 450 core

// Tessellation Control Shader 예시

// output control point 개수는 3개 (input default 값과 동일)
layout (vertices = 3) out;

void main(void)
{
    // Only if I am invocation 0 ...
    if (gl_InvocationID == 0)
    {
    // 내부의 테셀리이션 정도
        gl_TessLevelInner[0] = 5.0;  // inner tessellation level 을 5로 설정

    // 외부 테셀레이션 -> 0.1.2 는 각 변..?
        gl_TessLevelOuter[0] = 5.0;
        gl_TessLevelOuter[1] = 5.0;
        gl_TessLevelOuter[2] = 5.0;
    }
    // Everybody copies their input to their output
    // gl_in 이라는 input 값을 gl_out 에 그대로 복사한다.
    // gl_InvocationID 은 gl_in, gl_out 의 index 로 사용된다.
    // zero-based index of 제어점 (control piont)
    // 즉, 현재 TCS 가 처리중인 patch 내에서의 제어점의 index
    gl_out[gl_InvocationID].gl_Position =
        gl_in[gl_InvocationID].gl_Position;
}
*/
#pragma endregion

#pragma region Tessellation (Tesellation Engine)

/*
>> Patch 로 표현되는 high-order surface 를 point, line, traangle 등ㅇ
더 쉬운 primitive 로 변환하는 과정.

즉, 실제 output vertex 들을 생성하는 과정이다.
path 를 단순한 primitive 로 변환

Tessellation Engine 이 patch 를 받아들이기 전에
TCS 가 incoming control pint 들을 처리하고
patch 를 break down 할 때 사용하는 
tesselatoin factor 를 결정한다.

tessellation engine 이 output primitive 를 결정하고 나면
정점들은 tesellation evaluation shader 에서 선택된다.

즉, Tessesllation Engine 은 TES(테셀레이션 평가 쉐이더) 에게
필요한 parameter 들을 생성하는 일을 한다.

TES 에서 그리고 이 값들을 활용하여 실제 결과 primitive 로
변환해주고, rasterization 이 될 준비를 해준다.
*/
#pragma endregion

#pragma region Tessellation Evolution Shader ( TES )

/*
Tesellation Engine이 실행되면 생성된 프리미티브를 나타내는
다수의 출력 버텍스(정점)이 생성됩니다.

TES 는 
테셀레이터에 의해 생성된 각 버텍스에 대해 한 번씩 호출됩니다.

즉, 테셀레이션 이후 생성된 정점들을 처리

테셀레이션 레벨이 높을수록 TES 는 
매우 많은 횟수로 실행될 수 있습니다.

따라서 복잡한 평가 셰이더와 높은 테셀레이션 레벨을 사용할 때는 
주의해야 합니다.

"#version 410 core                                                                 \n"
"                                                                                  \n"
// tessellation mode 를 설정하는 부분
// triangle : 삼각형으로 테셀레이션
// equal_spacing : 새롭게 생성되는 정점들은 
//                      Polygon edge들 사이에서 동일 간격으로 설정
// cw : 입력 순서 시계 방향
"layout (triangles, equal_spacing, cw) in;                                         \n"
"                                                                                  \n"
"void main(void)                                                                   \n"
"{                                                                                 \n"
// 테셀레이션 엔진을 거치면 새로운 정점들이 생성되는데, 
// 각 점에서의 attribute들을 직접 보간해줘야 한다

// gl_in[3]은 이 정점을 생성하는 데 사용된 3개의 정점을 나타낸다.
// TCS 에서 gl_out 부분과 일치한다.

// gl_TessCoord는 보간에 필요한 인자들
// 테셀레이터에 의해 생성된 버텍스의 중심 좌표
"    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
"                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
"                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
"}
*/
#pragma endregion


#pragma region Geomtery Shader

/*
rasterazation 직전, 마지막 shader 단계이다.
테셀레이션 단계 이후에 진행된다.

> 해당 shader 는 primitive 단위로 실행된다 (도형)

> 기하셰이더의 특징 중 하나는, shader 중 유일하게 
draw pipeline 에서 사용되는 데이터의 양을 늘리거나 줄일 수 있다.

테셀레이션도 물론 도형 개수를 늘릴 수 있으나
기존 도형 범위 안에서만 늘리는 것이지만

기하셰이더는 아예 새롭게 만들 수도 있다는 것이다.
즉, 아예 추가적인 정점들도 만들 수 있다는 것이다.

> primitive mode mid-pipeline 도 변경할 수 있다.
예를 들어, 삼각형을 input 으로 받아서 
여러 point 혹은 line 들로 출력해버릴 수도 있다.

반대로 여러 point 들로부터 삼각형을 만들어버릴 수 있다.

즉, primitive 모양도 변경해버릴 수 있는 것이다.

> 예시
#version 450 core

layout(triangles) in;
layout(points, max_vertices = 3) out;

void main(void)
{
    int i;

    for (i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
}

삼각형을, point 로 변경해버리고 있다.
(새로운 삼각형을 추가로 만드는 것 같지는 않다)
이를 통해 우리는 삼각형들의 정점을 보게 된다.

layout(triangles) in; 이란
우리가 삼각형을 input 으로 받는다는 것

layout(points, max_vertices = 3) out; 이란
기하 셰이더가 출력으로는 point 들을 만들어낼 것이고
최대 숫자는 3개라는 의미이다.

main 함수 안을 보면 gl_in 내용을 순회하는 것을 볼 수 있다.
이때 length 는 3이다. 왜냐하면 우리는 삼각형을 인자로 받기 때문이다.

기하 셰이더의 출력은 vertex shader 의 출력과 동일하게 된다.

특히 gl_Position 변수를 사용하여 resulting vertex 의 
position 을 세팅한다.

그 다음 EmitVertex 함수를 호출한다.
이것은 기하셰이더 출력에 vertex 를 생성하는 것을 의미한다.

기하셰이더는 자동으로 EncPrimitive()  함수를 shader 끝에서 부른다.

결과적으로 3개의 정점이 생겨나고 point 들로 그려지게 될 것이다.
*/
#pragma endregion


#pragma region Primitive Assembly, Clipping, and Rasterization

/*
자. 이제 정점들을 화면의 pixel 형태로 변화하는 작업이
진행된다.

1st : primitive assembly
- vertex shader 가 생성한 정점들을 primitive 로 조합한다.
ex) line , triangle 들로 조합한다.

2nd. clipping
- 각 정점들이 하나의 primitive 들로 구성되고 나면
화면에 보이지 않는 부분은 clipping 된다.
보통 'view port' 라고 불리는 사각형 영역으로 제한된다.

3rd. rasterization
- clipping 이 끝나면, primitive 들은 화면의 pixel 로 변환된다.
즉, 보여지는 대상들이 어떤 pixel 에 맺히는지를
결정한 다음

이 pixel 들은 fragment shader 로 넘어가게 된다.
*/
#pragma endregion

#pragma region Clipping

/*
>> Clipping

1) homo geneouse coordinate

- pipeline 앞쪽을 거치고 나면 정점들은 보통
"clip spze" 라고 불리는 공간에 놓이게 된다.

gl_Position 은 vec4 type 이다.
이를 보통 homogenouse coordinate 라고 부른다.

이는 각 축이 90도를 이루는 x,y,z 기반의
Cartesian space 와 다른 개념이다.

homogeneous coordinate 는 4개의 값으로 이루어진다.

project geometry 에서 사용되는 좌표 시스템이다.

2) perspective divistion

- 물론, pipeline 앞단의 결과물은 비록 4 개의 원소로 이루어진
homogeneouse coordinate 이지만, 
Clipping 과정은 Catesian Space 에서 이루어진다.


- 따라서 homogeneouse 좌표를
Cartesian 좌표로 변경하기 위해서
OPENGL 은 perspective division 을 진행한다.

이는 모든 4개의 원소를 마지막 원소 w 로
나누는 것이다.

이를 통해 바로 homoge neouse coordinate 를
Cartesian coordinate 로 변환할 수 있다.

그리고 마지막 w 원소는 1 이 된다.

지금까지의 모든 코드에서는 gl_Position 에서
w 을 1로 만들어왔기 때문에 특별히
더 고려할 필요가 없었던 것 뿐이다.

3) normalized device space

- projective division 이후, 결과 position 은
normalized device space 공간에 놓이게 된다.

OPENGL 에서는 
normalized device space 에서 눈에 보이는 영역은
z 은 0 ~ 1
x,y 은 -1 ~ 1 이다.

해당 영역안에 놓인 geometry 만이
유저들에게 보이게 된다.
그 영역 밖에 있는 대상들은 버려진다.

더 구체적으로 normalized device space 은
6개의 면으로 이루어진 3차원 공간 상의
육면체이고, 각 면은 "in" 과 "out" 이 존재한다.

자. 이제 다음 단계로 primitive 들을 보내기 전에

각 도형들의 정점들이 각 면을 기준으로
in, out 에 있는지를 판단한다.

만약 모든 정점이 out 에 있다면 해당 도형은
clip 된다.

하지만, 단 하나라도 in 영역에 있다면
해당 primitive 는 clip 되지 않고
다음 영역으로 넘어가게 된다.

일부만 걸치게 되는 primitive 는
별도로 처리가 되어야 한다.
*/
#pragma endregion


#pragma region Viewport Transformation

/*
Clipping 이후에는 모든 도형의 정점들은
x,y 가 -1 ~ 1 
z 가 0 ~ 1 의 normalized device space 에 놓이게 된다.

그러나 화면을 그려내는 window 는
(0.0) ~ (width, height) 의 좌표를 가지고 있다.

window 상에 화면을 그려내기 위해
viewport transform 을 진행한다. 

즉, normalize device space 에서 window space 로
변환하는 과정을 거친다는 것이다.

scale 과 bias 는 아래와 같은 함수들을 통해 설정할 수 있다.

glViewport() and glDepthRange()

{
    glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    glDepthRange(GLdouble nearVal, GLdouble farVal);
}
*/

#pragma endregion

#pragma region Culling

/*
유저 쪽으로 바라보는 front-face 삼각형만 그려내고
back face 삼각형은 그리지 않는다.

glEnable() with cap set to GL_CULL_FACE 을 통해
Culling 을 진행할 수 있다.

기본적으로는 back face culling 을 진행한다.

glCullFace() 을 통해서 back face, front face culling 여부를
설정할 수도 있다.
*/
#pragma endregion


#pragma region Rasterization

/*
이 과정은 line, 삼각형 들의 primitive 들에 의해
어떤 픽셀들이 맺히는지를 판단하는 과정이다.

보통 삼각형 내부에 존재하는 픽셀은, 맺히는 픽셀이라고 판단하게 된다.

각 과정은 parallel 하게 계산되어 성능을 높인다.
*/

#pragma endregion

#pragma region Fragment Shader

/*
마지막 셰이더 단계이다.

frame buffer 로 보내지기 전에
각 픽셀의 색상을 겾렁한다.

ex) 
#version 450 core

out vec4 color;

void main(void)
{
    color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
        cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
        sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),
        1.0);
}

gl_FragCoord 은  builtin 변수로
화면 상에서 각 픽셀의 색상을 의미하게 된다.
*/
#pragma endregion

#pragma region Framebuffer Operations

/*
Frame Buffer 는 opengl grahpic pipeline 의 마지막 단계이다.

화면에 보여지는 pixel 정보를 담을 수 도 있고
그외에 각 픽셀 별 값을 담는 메모리로도 활용될 수 있다.

보통 대부분의 platform 에서 desktop 에서 보여지는
window 는 해당 window system 에서 기본적으로 제공하는
default frame buffer 이다.

하지만 그외에 frame buffer 를 추가로 제작하는 것도 가능하다.
*/
#pragma endregion

#pragma region Pixel Operations

/*
fragment shader 가 결과물을 생성한 이후
window 에 쓰이기 전에 몇가지 연산이 발생한다.

ex) scissor test 
- 특정 영역에 대해서만 픽셀을 그리게 할 수 있다.

ex) stencil test
- 애플리케이션에서 제공하는 참조 값과 스텐실 버퍼의 내용을 비교
텐실 버퍼는 픽셀당 단일 4비트 값을 저장합니다.
스텐실 버퍼의 내용은 특별한 의미가 없으며 어떤 용도로든 사용될 수 있습니다.

ex) depth test
- stencil test 가 이루어진 이후 depth test 를 한다

- 픽셀의 z 좌표와 depth buffer 에 저장된 값을 비교해서, 현재 그리고자 하는
픽셀의 z 좌표가, depth buffer 에 저장된 값보다 가까우면 (z 기준 작으면 ?)
depth buffer 의 내용을 update 하는 것이다.

- depth buffer 는 stencil buffer 처럼 frame buffer 의 일부 메모리라고 생각하면 된다.
각 픽셀 별로 single value 만을 저장할 수 있다.

- 보통 depth buffer 안에 저장되는 값은 0 ~ 1 사이의 값이다.
0 은 가장 가까운 것. 1은 가장 먼 것 

ex) blending, logical operation
- 
*/
#pragma endregion


#pragma region Compute Shader

/*
계산 셰이더는 그래픽 파이프라인과 독립적인 과정이다.

각 c.s 는 'work item' 이라고 불리는 single unit of work 에서 작동한다.

해당 work item 들은 'local work group' 이라는 작은 group 으로 그룹화 된다.

이러한 work group 들의 collection 이 opengl compute pipeline 에 보내진다.

c.s 는 정해진 형태의 input, output 이런 것이 없다.

대신에, c.s 는 각 work item 이 어떤 일을 할지를 정의한다.

ex) 
#version 450 core

// local work group 의 크기가 32 * 32 work items.
layout(local_size_x = 32, local_size_y = 32) in;

void main(void)
{
    // Do nothing
}
*/

#pragma endregion
