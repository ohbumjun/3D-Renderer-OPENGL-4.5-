opengl 은 여러개의 데이터를 병렬적으로 처리하는데에 최적화 되어 있다.
따라서 작은 양의 데이터를 매번 opengl 쪽으로 넘겨주는 것은
비효율적이다.

opengl 측에서 데이터를 저장하고, 접근하기 위해서
2가지 형태의 data storage 를 사용한다.

1) buffer
2) texture

#pragma region BUFFERS 1) data store

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

#pragma endregion


#pragma region BUFFERS 2) binding 

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


#pragma endregion


#pragma region BUFFERS 3) buffer 생성 및 memory 할당

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

#pragma endregion
