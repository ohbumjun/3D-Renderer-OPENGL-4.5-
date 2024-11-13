
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



#pragma region BUFFERS 4) buffer 에 데이터 복사해오기


#pragma endregion