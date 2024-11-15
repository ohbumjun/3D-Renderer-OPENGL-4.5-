
/*
opengl 은 여러개의 데이터를 병렬적으로 처리하는데에 최적화 되어 있다.
따라서 작은 양의 데이터를 매번 opengl 쪽으로 넘겨주는 것은
비효율적이다.

opengl 측에서 데이터를 저장하고, 접근하기 위해서
2가지 형태의 data storage 를 사용한다.

1) buffer
2) texture
*/

#pragma region BUFFERS 1) data store

/*
1) data store 
buffer 은 선형 메모리 공간이다.
여러 목적으로 사용될 수 있다.

보통 buffer 들은 ''이름" 으로 식별된다.

buffer 를 사용하기 전에는
opengl 에게 특정 name 을 reserver 하고 
해당 이름들을 메모리 alloc 에 사용할 것이라고 해줘야 한다.

buffer object 를 위해 할당되는 메모리를 "data store" 라고 부른다.

buffer 의 data store 는, opengl 이 data 를 저장하는 곳이다.

opengl command 를 통해 data store 에 data 를 넣을 수 있다.

혹은 buffer 에 대한 포인터를 cpu 측에서 얻어와서
해당 buffer 에 직접 데이터를 쓸 수도 있다.
*/

#pragma endregion

#pragma region BUFFERS 2) binding 

/*
2) binding

buffer 의 이름을 가진 이후에는 opengl context 에 bind 시켜야 한다.
정확히는 opengl context 에 있는 "binding ponit" 에 attach 해야 한다.

binding point 는 "Targets" 라고도 불린다.

opengl 에는 많은 숫자의 buffer binding point 가 있다.

그리고 각 binding point 는 다른 용도를 지니고 있다.

예를 들어,

buffer 의 내용을 vertex shader 의 input 으로 사용할 수도 있고
shader 들이 사용할 변수들의 값을 저장하는 용도로 사용할 수도 있고
shader 들이 생산해내는 데이터들을 저장할 공간으로 사용할 수도 있다.

같은 buffer 를 동시에 여러 목적으로 사용할 수도 있다.
*/

#pragma endregion

#pragma region BUFFERS 3) buffer 생성 및 memory 할당

/*
* https://rvalueref.tistory.com/6
* 
* >삼각형이 기본단위이다.
* 삼각형을 'Primitive' 라고 부른다.
* 
* > 삼각형은 3개의 Vertex (꼭짓점) 으로 나타낼 수 있다.
* 
* > 각 꼭짓점은 "속성" 을 지닌다
ex) 각 점의 위치 정보, 색상 정보 등
      예를 들어 2차원 좌표에서 삼각형 2개를 표현하고 싶다. 각각 분홍색, 초록색

  1) 위치 정보 속성
  struct Vec2d {
        float x;
        float y;
    };

  2) 색상 정보 속성
  struct Color {
        float r;
        float g;
        float b;
    };

    // 꼭짓점 정보를 담을 수 있는 구조체
    struct Vertex {
        Vec2d position;
        Color color;
    };

    즉, 하나의 꼭짓점은, 2개의 속성 "위치, 색상" 을 지닌다.

* >> VBO (Vertex Buffer Object)
* 
// 삼각형 데이터
const Color pink_color = {1.0f, 0.765f, 0.765f};
const Color green_color = {0.545f, 1.0f, 0.345f};

const Vertex vertices[6] = {
    // 핑크색 삼각형
    Vertex{Vec2d{2.0f, 8.0f}, pink_color},
    Vertex{Vec2d{1.0f, 5.0f}, pink_color},
    Vertex{Vec2d{5.0f, 6.0f}, pink_color},

    // 연두색 삼각형
    Vertex{Vec2d{4.0f, 4.0f}, green_color},
    Vertex{Vec2d{6.0f, 1.0f}, green_color},
    Vertex{Vec2d{8.0f, 3.0f}, green_color},
};

위와 같이, 삼각형 각 꼭짓점들을 "배열" 에 넣었다고 해보자.
이와 같이 "꼭짓점 정보들을 담고 있는 배열 객체" 를 VBO 라고 한다.

그런데  VBO 에 담겨 있는 정보는, 형식이 정해져있지 않다.
따라서 OpenGL 에서는 VBO 만 가지고는, 해당 정보를 
삼각형들로 어떻게 해석할지 모른다.

따라서, VBO 를, 어떤 형태의 삼각형으로 해석해야 하는가에 대한
"메타 정보" 를 가지고 있는 "객체" 를 VAO 라고 한다.

>> glCreateBuffers
위의 삼각형 정보들을 활용하여 VBO 를 만들 수 있다.
// VBO 생성
GLuint my_vbo;
glCreateBuffers(1, &my_vbo);

이렇게 호출된 이후, 'my_vbo' 에는 새로 생성된 VBO 번호가 저장되어 있다.

>> glCreateVertexArrays
- VBO 은 꼭짓점 데이터를 그저 바이트 배열 형태로만 볼 뿐
내부 구조는 모른다.

VBO 데이터를 OpengL 에서 해석하고 사용하기 위해서는
VAO 를 사용해야 한다.

VAO 구조는 기본적으로
1) 속성 : 꼭짓점의 속성 ex) 0 : 위치, 1 : 색상
2) VBO 바인딩 
이라는 2가지의 슬롯이 존재한다.

VBO 바인딩 슬롯에는 VBO를 연결할 수 있다.

각 속성에는 최대 1개의 VBO 을 바인딩 할 수 있다.

그리고 VBO 바인딩에 연결된 VBO 을 어떻게
해석해야 하는지에 대한 형식(format) 을 설정할 수 있다.


>> glVertexArrayVertexBuffer(
    GLuint vaobj,           // vao 번호
    GLuint bindingindex, // 몇번째 vbo 바인딩 슬롯에 바인딩 할 것인가.
    GLuint buffer,           // 바인딩할 vbo 번호
    GLintptr offset,         // vbo 의 몇번째 바이트부터 사용 ?
    GLsizei stride)          // 꼭짓점 사이의 간격
- VBO 을 VAO 에 있는 VBO 바인딩 슬롯에 연결하는 함수 이다.

>> glEnableVertexArrayAttrib() 함수
- 이제 VAO 에 바인딩한 VBO 을, "속성" 과 연결시키고자 한다.
연결하기 이전에 '속성' 을 "활성화 enable" 해야 한다.

2번째 인자에는 몇번째 속성을 활성화할 것인지를 넘겨준다.

// 0번 attribute로 position을 설정할 것임
const int POSITION_ATTRIBUTE_INDEX = 0;

// my_vao의 0번 attribute 활성화
glEnableVertexArrayAttrib(my_vao, POSITION_ATTRIBUTE_INDEX);

>> glVertexArrayAttribBinding
- 속성과 VBO 바인딩을 연결해주는 함수이다.

// my_vao의 0번 attribute로 my_vbo의 데이터를 바인딩
glVertexArrayAttribBinding(
    my_vao, 
    POSITION_ATTRIBUTE_INDEX,   // 속성의 번호
    MY_VBO_BINDING                  // VBO 바인딩 번호
);

>>  // my_vao의 0번 attribute에 바인딩된 vbo (my_vbo)를 어떻게 해석할지(format)를 설정
glVertexArrayAttribFormat(
        my_vao,                      // VAO 번호
        POSITION_ATTRIBUTE_INDEX,    // VAO attribute 번호
        2,                           // 벡터의 원소 개수
        GL_FLOAT,                    // 원소의 타입
        GL_FALSE,                    // normalized
        offsetof(Vertex, position)); // Vertex 내의 position 멤버의 위치

여기까지 해서 끝이 아니다. 
지금까지 한 것은, 0 번 속성에서 my_vbo 을 사용하라는 명령을
내린 것이다.

VAO 에서는 아직 my_vbo 의 "형식 format" 을 모른다.
위 함수를 통해 "형식" 을 지정해주어야 한다.


>> 예시 : 1번 속성에 color를 설정

// 1번 attribute로 color를 설정할 것임
const int COLOR_ATTRIBUTE_INDEX = 1;

// my_vao의 1번 attribute 활성화
glEnableVertexArrayAttrib(my_vao, COLOR_ATTRIBUTE_INDEX);

// my_vao의 1번 attribute로 my_vbo의 데이터를 바인딩
glVertexArrayAttribBinding(my_vao, COLOR_ATTRIBUTE_INDEX, MY_VBO_BINDING);

// my_vao의 1번 attribute에 바인딩된 vbo (my_vbo)를 어떻게 해석할지(format)를 설정
glVertexArrayAttribFormat(my_vao,                     // VAO 번호
                            COLOR_ATTRIBUTE_INDEX,    // VAO attribute 번호
                            3,                        // 벡터의 원소 개수
                            GL_FLOAT,                 // 원소의 타입
                            GL_FALSE,                 // normalized
                            offsetof(Vertex, color)); // Vertex 내의 color 데이터의 위치
*/

/*
opengl 에게 memory 를 할당하라고 얘기하기 전에
먼저 buffer object 를 만들어야 한다.

해당 buffer object 는 memory allocation 을 표현한다. 
각 buffer object 는 GLuint 변수로 표현된다.

즉, 메모리 할당을 표시. 식별하는 개념이 buffer object 이고
GLuint 라는 "name" 혹은 "이름" 을 가진다.

>> void glCreateBuffers(GLsizei n, GLuint* buffers);

해당 함수를 통해 buffer object 를 만들 수 있다.

n : 만들고자 하는 buffer object 의 개수
buffers : 만들어진 buffer object 들의 이름을 저장할 GLuint 배열

만약 1개의 buffer object 만을 만들고자 한다면 
n 은 1 이 되고, buffers 는 1개의 GLuint 변수를 가리키는 포인터가 된다.

*/

/*
>> void glBindBuffer(GLenum target, GLuint buffer);

위에서 buffer object 를 얻어낸 이후

현재 opengl context 에 buffer object 들을 bind 시켜야 한다. 

>> void glBufferStorage(GLenum target,
    GLsizeiptr size,
    const void* data,
    GLbitfield flags);

void glNamedBufferStorage(GLuint buffer,
    GLsizeiptr size,
    const void* data,
    GLbtifield flags);

buffer object 를 실제 사용하기 전에

buffer object 들의 "data store" 를 메모리 할당해야 한다.

다른 말로 하면 buffer object 가 표시하는 "메모리" 공간을
gpu 측에 할당하는 것이다.

이 과정은 위와 같이 glBufferStorage, glNamedBufferStorage 함수를
통해 이루어진다.

>> size resize ?
한번 할당한 buffer object 의 data store 는 resize 될 수 없다.
즉, 만약 resize 를 하고 싶다면
기존 buffer 를 delete 한 다음에
새로운 buffer 를 만들고
거기에 새로운 data 를 할당해야 한다.

다시 말해서, data store 내의 content 및 내용은 변경될 수 있다.
하지만 메모리 크기는 변경이 불가능하다

>> flags 인자

flags 인자는 buffer object 의 사용 목적을 나타낸다.
즉, 내가 어떤 형태 혹은 어떤 크기의 메모리를 할당할 것인지에 대한
정보를 지니고 있다.

* GL_DYNAMIC_STORAGE_BIT
- update the contents of the buffer directly
- once for every time that you use the data

즉, 해당 flag 는 buffer 의 내용을 직접 업데이트 할 수 있게 해준다.
만약 해당 flag 가 없다면 opengl 은 buffer 의 내용을 업데이트 할 수 없다.

그래서 glBufferSubData()  함수를 통해 buffer 의 내용을 업데이트 할 수 없다.

* GL_MAP_READ_BIT, 
* GL_MAP_WRITE_BIT, 
* GL_MAP_PERSISTENT_BIT, 
* GL_MAP_COHERENT_BIT

buffer 의 data store 을 어떻게 mapping 시킬 것인지를 결정한다.

이때 mapping 이란, opengl 측 data store 의 포인터, 즉
메모리의 주소를 직접 얻어오는 것을 의미한다.

GL_MAP_READ_BIT : 해당 메모리에서 데이터를 읽고 싶다

GL_MAP_WRITE_BIT : 해당 메모리에 데이터를 쓰고 싶다

GL_MAP_PERSISTENT_BIT : 해당 메모리를 지속적으로 사용하고 싶다
- 즉, 현재 command buffer 가 끝나도 해당 메모리를 계속 사용하고 싶다는 것
- 여러번 rendering frame 에서 접근되어야 할 경우 사용한다.
그래서 다른 drawing command 를 사용할 때에서 계속 "mapped" 된
상태로 놔두게 된다.

이는 성능에 영향을 주기 때문에 정말 필요한 경우에만 사용해야 한다.

GL_MAP_COHERENT_BIT : 해당 메모리를 coherent 하게 사용하고 싶다
- 버퍼에 대한 수정이 opengl 에 즉시 반영되도록 보장하는 것.
- 이는 버퍼의 내용을 update 하고, 렌더링된 출력에 즉시
반영되어야 하는 "real time rendering" 에 중요하다.
- 이것을 만약 세팅하지 않았다면, 해당 buffer 을 "unmap " 하지
않았는데도 불구하고 opengl 에게 data 를 썼다는 것을 알려줘야 한다.

*/

/*
>> 예시
// The type used for names in OpenGL is GLuint
GLuint buffer;

// Create a buffer
glCreateBuffers(1, &buffer);

// Specify the data store parameters for the buffer
glNamedBufferStorage(
                buffer,             // Name of the buffer
                1024 * 1024,        // 1 MiB of space
                NULL,               // No initial data -> 해당 인자를 통해 초기값 설정 가능
                GL_MAP_WRITE_BIT);  // Allow map for writing

// Now bind it to the context using the GL_ARRAY_BUFFER binding point
glBindBuffer(GL_ARRAY_BUFFER, buffer);

여기에서 "GL_ARRAY_BUFFER" 의 경우,
opengl 에게 해당 buffer 를 "vertex data" 를 담는데에 사용할 것이다 라고
제시하는 것이다.

물론, 해당 buffer 를 다른 target(binding point) 에 bind 시킬 수도 있다.
*/

#pragma endregion

#pragma region BUFFERS 4) buffer 데이터 일부 변경하기

/*
*  >> buffer 에 data 를 가져오는 다른 방ㅂ버은
* opengl 에게 buffer 를 준 다음, 해당 buffer 에 data 를 copy 하라고 하는 것이다.

glBufferSubData() or glNamedBufferSubData() 함수들을 통해서
buffer 에 넣고자 하는 data size , 
우리가 접근하고자 하는 buffer 내 offset ,
buffer 에 넣어져야할 메모리 내 data 에 대한 포인터
를 인자로 전달하면 된다.

>> void glBufferSubData(GLenum target,
                     GLintptr offset,
                     GLsizeiptr size,
                     const GLvoid * data);
void glNamedBufferSubData(GLuint buffer,
                          GLintptr offset,
                          GLsizeiptr size,
                          const void * data);

>> glBufferSubData

- glBufferSubData() 을 이용해서 buffer object 를 update 하기 위새서는
opengl 에게 , 해당 buffer object 에게 내가 데이터를 쓸 것이라고 얘기해줘야 한다.

이를 위해서 glBufferStorage() or glNamedBufferStorage() 함수를 호출할 때
GL_DYNAMIC_STORAGE_BIT 을 flag 에 포함시켜야 한다.

즉, glBufferStorage 와 glNamedBufferStorage 함수는
전체 buffer 를 할당 혹은 재할당한다

반면 glBufferSubData 와 glNamedBufferSubData 함수는
기존 buffer 내에서 특정 부분만을 "modify" 한다.
전체 buffer 를 다시 할당할 필요 없이 일부분만을 변경한다는 것이다.

위 인자 중에 glBufferSubData() 의 'target ' 변수와
glNamedBufferSubData() 의 'buffer' 변수는
binding point 를 의미한다
ex) GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER

즉, 어떤 유형의 binding point 가 함수의 영향을 받을 것인지를 결정한다.
*/

#pragma endregion

#pragma region BUFFERS 5) opengl 로부터 buffer data store 직접 얻어오기(mapping buffer)

/*
* >> 
(Map)
void *glMapBuffer(GLenum target,        // 특정 binding point 지정
                  GLenum usage);
void *glMapNamedBuffer(GLuint buffer, // buffer object 의 이름 지정
                       GLenum usage);

(Unmap)
void glUnmapBuffer(GLenum target);
void glUnmapNamedBuffer(GLuint buffer);
* >> 예시

// This is the data that we will place into the buffer object
static const float data[] =
{
     0.25, -0.25, 0.5, 1.0,
    -0.25, -0.25, 0.5, 1.0,
     0.25, 0.25, 0.5, 1.0
};

// Get a pointer to the buffer's data store
void * ptr = glMapNamedBuffer(buffer, GL_WRITE_ONLY);

// Copy our data into it...(vertex 정보를 채워준다)
memcpy(ptr, data, sizeof(data));

// Tell OpenGL that we're done with the pointer
glUnmapNamedBuffer(GL_ARRAY_BUFFER);
*/

/*
* >> glMapBuffer 함수가 glBufferSubData 함수보다 유용할 수 있는 이유

buffer object 에 upload 하고 싶은 매우 큰 data 가 있다고 사정하다.
해당 data 는 동적으로 만들어질 수도 있고, file 로 부터 read 될 수도 있다.

1) glBufferSubData 을 이용하는 방법
- data 를 임시로 저장할 memory 를 할당한다
- data 를 생성 혹은 file 로부터 read 하여 임사 memory 에 채운다
- glBufferSubData 함수를 통해 buffer object 에 "copy" 한다.

2) glMapBuffer 함수를 이용하는 방법
- buffer 를 map 한다. 즉, buffer 의 data store 에 대한 pointer 를 얻어온다.
- data 를 바로 map 한 pointer 에 쓰거나, file 로부터 데이터를 읽어서
 바로 pointer 에 적용하낟..
- buffer 를 unmap 한다.

즉, 불필요한 copy 를 막을 수 있다.
*/

/*
* >> glMapBufferRange, glMapNamedBufferRange 함수
* 
* glMapBuffer 와 glBufferSubData 은, 전체 buffer 를 mapping 하는 데에 유용하다.
* 하지만 한편으로는 덜 효율적이기도 하다. 전체를 mapping 하기 때문에
* 
* 특정 부분에 대한 mapping 이 필요한 경우에는 
* glMapBufferRange, glMapNamedBufferRange 함수를 사용하면 된다.
* buffer 에 특정 부분만을 mapping 한다.
* 
* >> 
void *glMapBufferRange(GLenum target,
                       GLintptr offset,
                       GLsizeiptr length,
                       GLbitfield access);

void *glMapNamedBufferRange(GLuint buffer,
                            GLintptr offset,
                            GLsizeiptr length,
                            GLbitfield access);

여기서 'GLbitfield' 은 어떤 식으로 mapping 할 것인지를 결정한다.

GL_MAP_READ_BIT: buffer 로부터 읽고 싶다

GL_MAP_WRITE_BIT:  buffer 에 쓰고 싶다

GL_MAP_PERSISTENT_BIT, GL_MAP_COHERENT_BIT :
    - glbufferStorage() 함수 인자와 비슷한 의미를 지닌다.

>> glBufferStorage() 와 flag 호환
- glBufferStorage() 함수를 통해 buffer object 를 만들 때
GL_MAP_WRITE_BIT 을 flag 로 설정했다면,
해당 buffer object 를 mapping 할 때에도
GL_MAP_WRITE_BIT 을 flag 로 설정해야 한다.

즉, 위 4개의 flag 종류들은, 항상 glBufferStorage() 와 호환되어야 한다.

>> glMapBufferRange 가, glMapBuffer 보다 더 유용하다.
*/

#pragma endregion

#pragma region BUFFERS 6) buffer object clear 하기
/*
* >> buffer 를 특정 constant value 로 채우기 위해서
* glClearBufferData, glClearNamedBufferData 함수를 사용할 수 있다.

void glClearBufferSubData(
    GLenum target,   // type of buffer to clear ex) vertex buffer
    GLenum internalformat,
    GLintptr offset,    // 단위 : byte
    GLsizeiptr size,    // 단위 : byte
    GLenum format,  // ex) GL_RED, GL_RG, GL_RGB, or GL_RGBA
                                  채널을 명시하는 부분dlek. 1,2,3,4 채널
    GLenum type,     // data type ex) GL_FLOAT, GL_INT, GL_UNSIGNED_INT
    const void * data // data 를 clear 시킬 value 를 담고 있는 memory 의 pointer
);

void glClearNamedBuffeSubData(GLuint buffer,
                              GLenum internalformat,
                              GLintptr offset,
                              GLsizeiptr size,
                              GLenum format,
                              GLenum type,
                              const void * data);

ex) red 색상으로 buffer 를 채우고자 한다면

glClearBufferSubData(GL_ARRAY_BUFFER, GL_RGBA, 0, bufferSize, GL_RGBA, GL_FLOAT, &redColor);

 >> 
*/
#pragma endregion

#pragma region BUFFERS 7) buffer ~ buffer 사이의 데이터 이동
/*
>> buffer ~ buffer 사이의 데이터 이동이 필요한경우

1) share data
- 두개의 buffer object 가 같은 data store 를 공유하도록 할 수 있다.

2) copy data
- 한 buffer object 의 data store 를 다른 buffer object 로 복사할 수 있다.

3) data manipulation
- 특정 buffer data 는 연산을 위해 활용하고
 그 결과를 다른 buffer 에 저장할 수 있다.
*/

/*
* >> 아래 2개의 함수는 buffer ~ buffer 사이의 데이터 이동을 위해 사용된다.

void glCopyBufferSubData(
    GLenum readtarget, // source buffer object 의 binding point
    GLenum writetarget,// destination buffer object 의 binding point 
                                // 단, readtarget 에서 writetarget 으로 "copy" 는 안된다.
                                // ex) 둘다 GL_ARRAY_BUFFER 로 설정하면 안된다.
    GLintptr readoffset,
    GLintptr writeoffset,
    GLsizeiptr size
);

void glCopyNamedBufferSubData(
    GLuint readBuffer,
    GLuint writeBuffer,
    GLintptr readOffset,  // source buffer 에서 어떤 부분을 읽을 것인가
    GLintptr writeOffset, // destination buffer 에서 어떤 부분에 쓸 것인가
    GLsizeiptr size
);
*/


/* 
>> GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER Targets:

- buffer 사이에 data 를 copy 할 때, 의도하지 않은 side effect 를 막기 위해서
추가된 binding point (target) 들이다.

- 사용 예시) 
// Bind the source buffer to GL_COPY_READ_BUFFER
glBindBuffer(GL_COPY_READ_BUFFER, sourceBuffer);

// Bind the destination buffer to GL_COPY_WRITE_BUFFER
glBindBuffer(GL_COPY_WRITE_BUFFER, destinationBuffer);

// Copy the data from the source buffer to the destination buffer
glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, bufferSize);
*/

#pragma endregion

#pragma region BUFFERS 8) Buffer 에서 Vertex Shader 로 데이터 전달하기

// https://rvalueref.tistory.com/6

/*
* buffer object 들로부터 vertex shader 로 데이터를 동적으로 만들어서
* 전달할 수 있다.
* 
* ex) 
* - 셰이더를 재 컴파일 하지 않고도, vertex data 를 수정할 수 있다.
* - gpu memory 측에 큰 데이터를 저장할 수도 있고, 거기에 접근할 수도 있다.
*/

/*
* >> VAO 의 역할 (Vertex Array Object)
* - 정점 attribute 의 'state' 을 저장하는 container 다.
* 즉, 정점 attribute 정보를 저장하는 공간이라고 생각하면 된다.
* 
* 그래서 어떻게 buffer object 들로부터 vertex data 를 해석하고
* 가져올 것인지에 대한 정보를 담고 있다.
* 
* VAO 를 OpenGL context 에 bind 시키면,
* rendering 을 위해서 어떻게 정점 데이터를 처리할지를
* 알려주는 것이다.
* 
* 1) VAO 생성
* glCreateVertexArrays(1, &vao)
- Allocates memory for a VAO and assigns its handle to the vao variable
*
* 2) VAO Bind 시키기
* glBindVertexArray(vao) 
* -- 자. 여기서부터 이제 VAO state 정보를 채워줄 수 잇다.

* vertex shader 의 데이터를 하드 코딩하기보다는 
* vertex attribute 을, 
* OpenGL 에게, 우리가 제공하는 buffer object 에 저장된
* 정보로, 채워달라고 요청할 수 있다.

*/

/* >> glVertexArrayAttribBinding

void glVertexArrayAttribBinding(
        GLuint vaobj,           // VAO 에 대한 handle

        GLuint attribindex,   // shader 에서 vertex attribute 의 index
                                      // 해당 index 는 vertex shader 에서
                                       선언한다.

        GLuint bindingindex // binding 하고 있는 vertex buffer 의 index
                                     // 해당 index 는 glBindBuffer() 함수를 통해
                                     // binding 한 buffer object 에 대응된다.
);

shader 상의 vertex 속성 ~ vbo (buffer) 을 연결해주는 것

즉, 해당 VAO 를 활용하여 draw 를 하게 되면

1) VAO Bind
- 특정 VAO 를 activate 한다.

2) Fetch Vertex Data
- 각 vertex 마다, vertex atrribute 의 binding index 를 확인한다.

3) Access the Buffer
- 'bindingindex' 를 가진 buffer object 에서 data 를 가져온다.

4) Send Data to the Shader
- vertex shader 의 input 으로 넘겨준다.
*/

/*
* >> void glVertexArrayVertexBuffer(
        GLuint vaobj,               // vertex array object handle (vao)
        GLuint bindingindex,    // vertex buffer binding index 
                                        // glVertexArrayAttribBinding() 에서 사용한 index 와 일치해야 한다.
                                    // 몇버째 VBO 바인딩 슬롯에 바인딩 할 것인지
        GLuint buffer,    // vertex data 정보를 담은 buffer object
        GLintptr offset,  // buffer object 내에서의 byte offset (첫번째 정점 데이터 시작 위치)
        GLsizei stride     // 각 정점 데이터 사이의 byte offset
);

VAO 에 있는 VBO 바인딩 슬롯에, VBO 을 연결해주는 함수

대략적인 VBO 구조를 알려준다.

하지만, 더 구체적인 vbo 의 내용은 glVertexArrayAttribFormat 함수를
통해 알려주어야 한다.

*/

/*
* void glVertexArrayAttribFormat(GLuint vaobj,
    GLuint attribindex,  // 속성 idx
    GLint size,              // 각 정점 데이터의 원소 개수
    GLenum type,         // 데이터 타입

    // shader 로 넘겨줄 값을 정규화 시킬 것인가에 대한 변수이다.
    // GL_FLOAT 에 대해서는 적용되지 않는다.
    // GL_UNSIGNED_BYTE  혹은 GL_INT 에 대해서만 적용된다.
    // 예를 들어, GL_UNSIGNED_BYTE  를 사용하고, normalized 를 true 로 설정하면
    // 0 ~ 255 사이의 값을 0.0 ~ 1.0 사이 "FLOAT" 으로 변환한다.
    // 즉, vertex shader input 으로 들어가기 전에 0 ~ 1 사이로 변환 되므로
    // vetex shader 측에서는 0.0 ~ 1.0 사이의 float 값으로 input 을 받게 되는 것
    // 반면 해당 인자를 false 로 세팅하면
    // vertex shader 는0 ~ 255 사이의 float 값으로 바라보게 될 것이다.
    GLboolean normalized,
    GLuint relativeoffset);
*/

/*
>> void glEnableVertexAttribArray(GLuint index);
- 속성을 활성화 해주는 함수이다.

// First, bind a vertex buffer to the VAO
glVertexArrayVertexBuffer(vao,                  // Vertex array object
                          0,                    // First vertex buffer binding
                          buffer,               // Buffer object
                          0,                    // Start from the beginning
                          sizeof(vmath::vec4)); // Each vertex is one vec4

// Now, describe the data to OpenGL, tell it where it is, and turn on automatic
// vertex fetching for the specified attribute
glVertexArrayAttribFormat(vao,                  // Vertex array object
                          0,                    // First attribute
                          4,                    // Four components
                          GL_FLOAT,             // Floating-point data
                          GL_FALSE,             // Normalized - ignored for floats
                          0);                   // First element of the vertex

glEnableVertexArrayAttrib(vao, 0);

이제 opengl 은, vetex shader 에서의 0번째 속성을
VAO 에 binding 된 buffer 메모리로부터 읽어와서 해석할 것이다.

ex) 

#version 450 core

layout (location = 0) in vec4 position;

void main(void)
{
    gl_Position = position;
}
*/

/*
* >> void glDisableAttribArray(GLuint index);

- vetex attribute (속성) 값을 buffer object 의 데이터로 채우는 것을 마쳤다면
해당 속성을 disable 할 때 사용하는 함수 이다.
*/

#pragma endregion

#pragma region BUFFERS 9) vetex shader 에 여러 input 전달하기 

/*
* >> GLint glGetAttribLocation(GLuint program,
                          const GLchar * name);
 
넘겨준 속성이 몇번째 index 인지를 반환하는 함수이다.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
 
 예를 들어, "position" 을 2번째 인자로 넘겨주면 
 '0' 을 리턴할 것이다.

 만약 내가 속성별 index 를 specify 했다면
 내가 specify 한 index 를 리턴할 것이다.

 만약 index 를 specify 하지 않으면
 opengl 이 자동으로 index 를 할당할 것이다.
*/

/*
* >> vertex shader input 과 어플리케이션의 data 를 연결하는 방법

1) seperate attribute
- 서로 다른 속성 (ex. 위치, 색상) 은 서로 다른 buffer object 에 저장된다.
각 attribute 은 각자의 buffer 를 가지고 있다.

2) interleaved attribute
- 하나의 정점을 위한 모든 속성들이 single buffer object 에 저장된다.


>> separate vertex attributes 예시
- glCreateBuffers() 함수를 통해 2개의 서로 다른 buffer object 를 만들고 있다.
- 그리고 각 속성에 대해 glVertexArrayAttribBinding() 을 호출

GLuint buffer[2];
GLuint vao;

static const GLfloat positions[] = { ... };
static const GLfloat colors[] = { ... };

// Create the vertex array object
glCreateVertexArrays(1, &vao)

// Get create two buffers (2개 버퍼 동시 생성)
glCreateBuffers(2, &buffer[0]);

// Initialize the first buffer
// - allocate memory
// - copy data 
glNamedBufferStorage(buffer[0], sizeof(positions), positions, 0);

// Bind it to the vertex array - offset zero, stride = sizeof(vec3)
// vao 에 있는 vbo 바인딩 슬롯에 vbo 바인딩
glVertexArrayVertexBuffer(vao, 0, buffer[0], 0, sizeof(vmath::vec3));

// Tell OpenGL what the format of the attribute is
glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

// Tell OpenGL which vertex buffer binding to use for this attribute
// 속성과 vbo 연결
glVertexArrayAttribBinding(vao, 0, 0);

// Enable the attribute
glEnableVertexArrayAttrib(vao, 0);

// Perform similar initialization for the second buffer
glNamedBufferStorage(buffer[1], sizeof(colors), colors, 0);
glVertexArrayVertexBuffer(vao, 1, buffer[1], 0, sizeof(vmath::vec3));
glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
glVertexArrayAttribBinding(vao, 1, 1);
glEnableVertexAttribArray(1);

>> interleaved vertex attributes 예시
- glCreateBuffers() 을 한번만 호출해서 하나의 buffer object 만을 만든다
- glVertexArrayAttribFormat 만 2번 호출해서, vertices 내에 서로 다른 
  속성의 offset 을 연결만 시켜준다. (맨 처음 https://rvalueref.tistory.com/6 예시)

GLuint vao;
GLuint buffer;

static const vertex vertices[] = { ... };

// Create the vertex array object
glCreateVertexArrays(1, &vao);

// Allocate and initialize a buffer object
glCreateBuffers(1, &buffer);
glNamedBufferStorage(buffer, sizeof(vertices), vertices, 0);

// Set up two vertex attributes - first positions
// 속성과 vbo 바인딩 연결
glVertexArrayAttribBinding(vao, 0, 0);
glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, x));
glEnableVertexArrayAttrib(0);

// Now colors
glVertexArrayAttribBinding(vao, 1, 0);
glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, r));
glEnableVertexArrayAttrib(1);

// Finally, bind our one and only buffer to the vertex array object
glVertexArrayVertexBuffer(vao, 0, buffer);
*/

/*
* >> glVertexArrayAttribBinding 활용 방법
- 만약 매우 많은 geometry 정보들이
서로 다른 buffer 에 저장되어 있고, 비슷한 format 을 가지고 있을 때

그것들을 그릴 때, 그저, glVertexArrayAttribBinding() 함수를 통해
속성에 연결된 buffer 만 switch 해주고 draw 해도 된다.
*/
#pragma endregion

#pragma region UNIFORM
/*
* >> Uniform 기본 용도
- vertex shader 뿐만 아니라 모든 shader 단계 쪽에
data 를 직접 보내줄 때 사용할 수 있다.

2 개 형태의 uniform 이 있다.
1) default block
2) uniform block
*/
#pragma endregion

#pragma region UNIFORM 1) Default Block Uniforms

/*
Vertex attribute 는 각 정점 별로 다른 값을 pass 하는데에 사용했다면
uniform 은 모둔 vertex 에 동일하게 적용되는 값을 넘겨주는데 사용할 수 있다.
ex) transform matrix


>> 예시
uniform float fTime;
uniform int iIndex;
uniform vec4 vColorValue;
uniform mat4 mvpMatrix;

shader 상에 해당 uniform 에 값을 쓸 수는 없다.
하지만 아래와 같이 초기값을 지정하는 것은 가능하다.

uniform int answer = 42;
*/

#pragma endregion

#pragma region UNIFORM 2) Arranging Your Uniforms

/*
* >> Opengl  에는 shader 의 값을 쓰는 많은 함수들이 있다
이러한 함수들은, uniform 변수들을 'location' 을 통해 식별하고
접근한다.

아래와 같이 'layout qualifier' 라는 것을 이용하여
uniform 마다 location 을 할당할 수 있다.

layout (location = 17) uniform vec4 myUniform;

만약 이와 같이 lcoation 을 별도로 할당하지 않으면
opengl 이 대신 할당해준단.

>> GLint glGetUniformLocation(GLuint program,
                           const GLchar* name);
 해당 함수를 통해 uniform 의 location 을 얻을 수 있다.
ex) GLint iLocation = glGetUniformLocation(myProgram, "vColorValue");

만약 리턴값이 - 1 이라는 것은, 해당  uniform 이름 이 존재하지 않는다는 것이다.

또한 중요한 것이 있다. 내가 location 을 명시해준다고 하더라도
shader 중에 해당 uniform 을 사용하는 shader 가 단 하나라도 없다면
마찬가지로 uniform 이름은 "사라진다"
 */

#pragma endregion

#pragma region UNIFORM 3) Uniform 세팅하기

/*
* >> glUniform*() 함수를 통해 값을 세팅한다.
 
ex 1) 

layout (location = 0) uniform float fTime;
layout (location = 1) uniform int iIndex;
layout (location = 2) uniform vec4 vColorValue;
layout (location = 3) uniform bool bSomeFlag;

ex 1) 
glUseProgram(myShader);
glUniform1f(0, 45.2f);
glUniform1i(1, 42);
glUniform4f(2, 1.0f, 0.0f, 0.0f, 1.0f);
glUniform1i(3, GL_FALSE);

ex 2) 
uniform vec4 vColor;

ex 2)
GLfloat vColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

glUniform4fv(iColorLocation, 1, vColor);

ex 3) 

uniform vec4 vColors[2];

GLfloat vColors[4][2] = { { 1.0f, 1.0f, 1.0f, 1.0f } ,
                          { 1.0f, 0.0f, 0.0f, 1.0f } };

glUniform4fv(iColorLocation, 2, vColors);

*/

/*
* >> glUniform*() 인자 분석하기

count : '인자로 넘겨주는 pointer' 에 몇개의 matrix 가 존재하는가
            즉, 여러개의 matrix 값도 넘겨줄 수 있다.

GL_FALSE : transpose 여부
- 만약 넘겨주는 matrix 가 이미 column major 이라면, GL_FALSE 를 넘겨주면 된다.
  GL_TRUE 라고 하면, vertex shader 측으로 '복사' 하여 넘겨줄 때
  전치 행렬 을 넘겨준다.

  만약 계산한 matrx 가 row major 이라면 !

  https://stackoverflow.com/questions/33862730/row-major-vs-column-major-confusion

  row major, column major 에 대한 설명
*/
#pragma endregion

#pragma region UNIFORM 4) Uniform Blocks

/* >> 등장 배경
- shader 에서 정말 많은 uniform 변수들이 필요하게 될 수도 있는데
그것들을 일일히 glUniform 변수를 통해서 세팅하는 것은 매우 비효율적이다.
일부 uniform 은 매 프레임 변경이 필요할지 몰라도
일부는 맨 처음 한번만 초기화 하는 경우들도 있다.
즉, uniform 들을 묶어서 한번에 세팅 혹은 관리하는 것이 효율적이다.
이를 위해서 uniform block 이 나온 것이다.

>> 동작 원리
- 'uniform block' 은, uniform 들을 모아서, 특정 buffer object block 에
저장하는 것이다.

buffer object 는 지금까지 설명해온 개념 그대로다.

전체 uniform group 을 buffer binding 을 변경함으로써
한꺼번에 값을 세팅할 수도 있고
혹은 buffer content 를 변경함으로써 , group of uniform 들을 변경할 수도 있다.

다시 말해서, Uniform Buffer Object(UBO) 는
큰 크기의 uniform data 를 shader 측에 넘길 때 사용하는 개념이다.
*/

/*
* >> 예시
uniform TransformBlock
{
    float scale;            // Global scale to apply to everything
    vec3 translation;       // Translation in X, Y, and Z
    float rotation[3];      // Rotation around X, Y, and Z axes
    mat4 projection_matrix; // A generalized projection matrix to apply
                            // after scale and rotate
} transform;

'TransformBlock' 이라는 uniform block 을 선언하고 있다.

해당 uniform block 에 대응되는 buffer object 에 data 를 쓰고 싶다면
UBO 의 각 멤버의 location 을 알아야 한다.

ex)

OpenGL Shading Language
// Accessing the scale factor
float scaled_value = transform.scale * original_value;

// Applying the translation
vec3 translated_position = position + transform.translation;
*/

/*
* >> Uniform Block Object Data 가 생긴 모양 (Uniform Buffer Layout)

1) Standard

- redefined, standardized layout for uniform block 을 이용한다.

- 그저 buffer 에 데이터 복사 (UBO 멤버들이 특정 순서로 배열되어 있다고 가정)

- 편하지만, 비효율적일 수 있다. 멤버들 사이에 padding 이 존재할 수 있다.
이로 인해 buffer 가 실제 필요한 크기보다 커질 수 있다.

- 해당 방식이 보통 안전하다

2) Custom

- OpenGL 이 data 를 어디에 베치할지를 알아서 결정하게 하는 방식이다.
이는 매우 효과적인 shader 를 짤 수 있으나, 다른 의미로
application 이 각 데이터가 어디에 있는지를 알아야 한다는 의미이다.

- 이 경우, UBO 안에 있는 data 들은 'shared layout' 에 정렬되어 있다.
쉽게 말해 OpenGL 측에서 접근하느데에 있어 최적화된 방식으로
데이터를 정렬시켜서 사용한다는 것이다.

반대로 말하면 OpenGL 은 빠르고 효율적으로 데이터를 처리할지 몰라도
application 이 할일은 더 많다는 것이다.
*/

/*
* >> Standard 예시

std140 : UBO 내 data 들을 standard 방식으로 구성하는 방식
그냥 쉽게 말해서 우리 c++ 에서 struct 크기 align 맞추는 것이다.

멤버 중에서 가장 큰 값에 맞춰서 나머지 값들도 padding 줘서 맞추는 등의
정렬 방식을 선택한다는 것이다.

ex 1)

layout (std140) uniform ExampleBlock
{
                            // base alignment  // aligned offset
    float value;        // 4                // 0
    vec3 vector;        // 16              // 16  (16의 배수여야하므로 4->16)
    mat4 matrix;     // 16                  // 32  (0 열)
                            // 16               // 48  (1 열)
                            // 16               // 64  (2 열)
                             // 16              // 80  (3 열)
    float values[3]; // 16                  // 96  (values[0])
                            // 16              // 112 (values[1])
                            // 16              // 128 (values[2])
    bool boolean;    // 4                   // 144
    int integer;        // 4                // 148
};

ex 2) 

layout(std140) uniform TransformBlock
{
// Member                      base alignment offset  aligned offset
   float scale;                 // 4                    0       0
   vec3 translation;        // 16                   4       16
   float rotation[3];       // 16                   28      32 (rotation[0])
                                    //                              48 (rotation[1])
                                    //                              64 (rotation[2])
   mat4 projection_matrix;  // 16             80      80 (column 0)
                                    //                              96 (column 1)
                                    //                              112 (column 2)
                                    //                              128 (column 3)
} transform;

자. 이렇게 중간 중간 padding 도 들어가고 하게 된다.
이때 주의할 점은, 해당 buffer object memory 를 채우기 위해서
그냥 C Array 을 이용하여 memcpy 하면 안된다.
C Array 은 위와 같은 padding 없이  데이터를 채우려고 하고
이는 부정확한 데이터를 채우게 될 수 있다.
*/

/*
>> Offst 직접 명시하는 방법

ex 1) 
layout(std140) uniform ManuallyLaidOutBlock
{
    layout (offset = 32) vec4       foo;    // At offset 32 bytes
    layout (offset = 8) vec2        bar;    // At offset 8 bytes
    layout (offset = 48) vec3       baz;    // At offset 48 bytes
}  myBlock;

이와 같은 방식으로 직접 offset 을 명시할 수도 있다.
*/

/*
>> Offst 직접 명시 + align 키워드 사용하는 방법

align 키워드를 사용하면 데이터 타입의 정렬을 강제할 수 있다.

ex 1)
layout (std140, align = 16) uniform ManuallyLaidOutBlock
{
    layout (offset = 32) vec4       foo;    // At offset 32 bytes
    layout (offset = 8) vec2        bar;    // At offset 16 bytes
    layout (offset = 48) vec3       baz;    // At offset 48 bytes
}  myBlock;

align = 16: 전체 블록을 16바이트 경계에 맞춰 정렬합니다.

foo: vec4 타입이므로 16바이트 경계에 맞춰져 있으므로 offset = 32가 유효합니다.

bar: vec2 타입은 일반적으로 8바이트 경계에 맞춰지지만, 
    블록 전체가 16바이트 경계에 맞춰져 있으므로 
       offset = 8은 16바이트 경계로 올림되어 실제 오프셋은 16이 됩니다.

baz: vec3 타입은 16바이트 경계에 맞춰져야 하므로 offset = 48이 유효
*/

/*
>> void glGetUniformIndices(
    GLuint program,
    GLsizei uniformCount, // number of uniform
    const GLchar ** uniformNames,
    GLuint * uniformIndices // 해당 값으로 특정 uniform 의 indice 를 얻어올 수 있다.
);

위 함수는, UBO 내 멤버의 index 를 얻어오는 함수이다.
한번에 여러개 멤버에 대한 index 들을 얻어올 수 있는 구조이다.

예를 들어, UBO 멤버 2개에 대한 index 를 얻어오고 싶다면
uniformCount 는 2, uniformNames 는 
2개의 uniform 이름을 가진 배열을 넘겨주면 된다.

ex) 
static const GLchar * uniformNames[4] =
{
    "TransformBlock.scale",
    "TransformBlock.translation",
    "TransformBlock.rotation",
    "TransformBlock.projection_matrix"
};
GLuint uniformIndices[4];

glGetUniformIndices(program, 4, uniformNames, uniformIndices);
*/

/*
* >> void glGetActiveUniformsiv(GLuint program,
       GLsizei uniformCount,
       const GLuint * uniformIndices,
       GLenum pname,
       GLint * params);

UBO 멤버들의 index 를 얻어왔기 때문에 이제
각 멤버들의 buffer 내 location 을 얻어올 수 있다.

ex)
GLint uniformOffsets[4]; // Transform Block 의 4개 멤버에 대한 offset
GLint arrayStrides[4];      // Transform Block 의 4개 멤버에 대한 array stride
GLint matrixStrides[4];   // Transform Block 의 4개 멤버에 대한 matrix stride
glGetActiveUniformsiv(program, 4, uniformIndices,
                      GL_UNIFORM_OFFSET, uniformOffsets);
glGetActiveUniformsiv(program, 4, uniformIndices,
                      GL_UNIFORM_ARRAY_STRIDE, arrayStrides);
glGetActiveUniformsiv(program, 4, uniformIndices,
                      GL_UNIFORM_MATRIX_STRIDE, matrixStrides);

예를 드렁, offset 을 얻어오고 나면
glBufferSubData() 함수를 통해 해당 buffer 내 특정 offset 에 있는
값을 opengl 로부터 load 해서 올 수도 있다.

혹은 glMapBufferRange() 함수로 전체 buffer 에 대한 포인터를 얻어온 이후
해당 offset 에 직접 데이터를 쓸 수도 있을 것이다.

ex 1) TransformBlock.scale 에 3.0f 를 넣고 싶다면
// Allocate some memory for our buffer (don't forget to free it later)
unsigned char * buffer = (unsigned char *)malloc(4096);

// We know that TransformBlock.scale is at uniformOffsets[0] bytes
// into the block, so we can offset our buffer pointer by that and
// store the scale there.
*((float *)(buffer + uniformOffsets[0])) = 3.0f;
* 
ex 2)  TransformBlock.translation 에 1.0f, 2.0f, 3.0f 를 넣고 싶다면
// Put three consecutive GLfloat values in memory to update a vec3
((float *)(buffer + uniformOffsets[1]))[0] = 1.0f;
((float *)(buffer + uniformOffsets[1]))[1] = 2.0f;
((float *)(buffer + uniformOffsets[1]))[2] = 3.0f;

ex 3) TransformBlock.rotation 에 값 세팅하기
 
 // TransformBlock.rotations[0] is at uniformOffsets[2] bytes into
// the buffer. Each element of the array is at a multiple of
// arrayStrides[2] bytes past that.
const GLfloat rotations[] = {  30.0f, 40.0f, 60.0f };

// offset : 'rotation' member 의 starting offset 이 여기에 들어가 있다.
unsigned int offset = uniformOffsets[2];

for (int n = 0; n < 3; n++)
{
    *((float *)(buffer + offset)) = rotations[n];

// arrayStrides[2] 란, 지금 rotation array 내부에 element 사이에 존재하는
// stride 를 의미한다.
// 즉, float[3] 에서 각 float 들이 , 연속으로 붙어있지 않을 수 있다는 것이다.
    offset += arrayStrides[2];
}

ex 4) TransformBlock.projection_matrix 값 세팅하기

// The first column of TransformBlock.projection_matrix is at
// uniformOffsets[3] bytes into the buffer. 

// The columns are
// spaced matrixStride[3] bytes apart and are essentially vec4s.
// This is the source matrix - remember, it's column major.
const GLfloat matrix[] =
{
    1.0f, 2.0f, 3.0f, 4.0f,
    9.0f, 8.0f, 7.0f, 6.0f,
    2.0f, 4.0f, 6.0f, 8.0f,
    1.0f, 3.0f, 5.0f, 7.0f
};

for (int i = 0; i < 4; i++)
{
// uniformOffsets[3] 에 project_matrix 의 시작 offset 이 저장되어 있다.
// matrixStride[3] : matrix 가 4개의 vec4 로 구성되어 있는 상황에서
//                          matrix 내 각 vec4 사이의 간격이 얼마나 되는지를 나타낸다.
    GLuint offset = uniformOffsets[3] + matrixStride[3] * i;
    for (j = 0; j < 4; j++)
    {
        *((float *)(buffer + offset)) = matrix[i * 4 + j];
        offset += sizeof(GLfloat);
    }
}
*/

/*
* 결론
* - 자. 위와 같이 shared layout 을 사용하면, 코드도 길고 복잡하다
* 따라서 되도록 standar layout 을 사용하는 것을 추천한다.
*/

/*
* >> GLuint glGetUniformBlockIndex(
        GLuint program,
        const GLchar * uniformBlockName // "TransformBlock"
);

자. 지금까지는 UBO 내의 각 멤버 및 원소에 대한 얘기를 했었다.
이제는 UBO 전체에 data 를 쓰고자 하는 상황이라고 해보자.

즉, 특정 ubo 를 찾아와서 거기에 data 를 한꺼번에 쓰고 싶은 상황이다.
그러면 shader program 에 많이 존재하는 uniform block 중에서
원하는 uniform block 을 찾아와야 한다.

각 UBO 들은, 고유한 index 를 지닌다.
이 index 를 얻어오기 위해서 위 함수를 사용한다.

*/
#pragma endregion
