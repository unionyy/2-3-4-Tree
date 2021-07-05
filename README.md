# 2-3-4-Tree
2-3-4 Tree Simulator (C Language)

2019 알고리즘 과제, C언어로 작성한 2-3-4 Tree 입니다.

(입력 제한사항: 양의 정수, 중복 X)

## 구현환경 및 인터페이스

### 구현 환경
Windows Visual Studio 2017 (Windows Console Solution)

### 인터페이스
![Welcome](https://raw.githubusercontent.com/unionyy/2-3-4-Tree/main/images/welcome.png)

프로그램이 실행되면 Welcome message가 출력되고 모드를 선택할 수 있는 문구가 나오게 됩니다.

![interactive](https://raw.githubusercontent.com/unionyy/2-3-4-Tree/main/images/interactive.png)

1번을 선택할 경우, Interactive mode가 실행되고 그에 따른 안내 메시지가 출력됩니다.

i□를 입력할 경우, insert가 실행되며 value가 Tree에 삽입됩니다. 그리고 입력 받은 명령어가 한번 출력되며, 다음 줄에 전체 Tree가 출력됩니다.

s□를 입력할 경우, search가 실행되고, 입력 받은 명령어가 한번 출력됩니다. 그리고 value가 존재할 경우 다음 줄에 exist가 출력되고, 존재하지 않을 경우 다음 줄에 not exits 가 출력됩니다.

d□를 입력할 경우, delete가 실행되고, value가 Tree에서 제거됩니다. 만약 value가 Tree 내에 존재하지 않을 경우에는 아무것도 삭제하지 않습니다. 그리고 입력 받은 명령어가 한번 출력되며, 다음 줄에 전체 Tree가 출력됩니다.

e0을 입력할 경우, 프로그램이 종료됩니다.

![batch](https://raw.githubusercontent.com/unionyy/2-3-4-Tree/main/images/batch.png)

초기화면에서 2번을 선택할 경우, Batch mode가 실행되고 실행 parameter를 체크한 뒤에 자동으로 output.txt파일을 생성합니다.

실행 parameter가 없을 경우, 또는 실행 parameter로 입력 받은 이름을 가진 파일이 존재하지 않을 경우, 에러가 발생하고 프로그램이 종료됩니다.

![setting](https://raw.githubusercontent.com/unionyy/2-3-4-Tree/main/images/setting.png)

실행 parameter는 Visual Studio 2017의 경우, 디버그->속성->디버깅->명령 인수 탭에서 설정할 수 있습니다.
 
## Source의 전체 Structure 및 2-3-4 tree의 data structure
### Source Code & Data Structure
#### 2-4Tree.c
모든 기능은 “2-4Tree.c” 하나의 C파일 안에 구현되어 있습니다. 이 파일 안에는 main 함수를 제외한 총 17개의 함수가 상세한 주석과 함께 구현되어 있으며, 총 903줄로 이루어져 있습니다. Source Code의 전문은 C-Library를 제외한 어떠한 참고 자료 없이 직접 작성되었습니다.

#### 2-3-4 tree의 data structure

![structure](https://raw.githubusercontent.com/unionyy/2-3-4-Tree/main/images/structure.png)

2-3-4 tree의 data structure는 typedef으로 node를 만들어 구현되었습니다. 하나의 node는 parent node의 포인터, 4개의 child node 포인터의 배열, 3개의 int형 value의 배열로 이루어져 있습니다.

pNode는 자신의 parent node를 가리키며, root node의 경우 nullptr로 설정합니다.

cNode의 배열은 자신의 child node를 순서대로 가리킵니다. 2-node일 경우 cNode[2]와 cNode[3]은 nullptr로 설정하며, 3-node일 경우에는 cNode[3]을 nullptr로 설정합니다. Leaf node의 경우 모든 cNode를 nullptr로 설정합니다.

val의 배열에는 node의 value가 순서대로 저장됩니다. 값이 비어 있을 경우, -1이 저장됩니다. 즉, 2-node일 경우, val[0]을 제외한 모든 값이 -1이고, 3-node일 경우, val[2]가 -1로 설정됩니다.

### Functions
#### main
main 함수는 인터페이스 부분과 Batch mode 부분, 그리고 Interactive mode 부분으로 크게 나눌 수 있습니다. 인터페이스 부분에서는 사용자에게 초기 메시지를 보여주고, makeNode함수를 이용하여 2-4 Tree의 root를 생성합니다. 그리고 사용자에게 모드를 선택 받아 Batch mode를 실행시킬 지 Interactive mode를 실행시킬지 결정합니다.

Batch mode 부분에서는 Batch mode 관련 메시지를 출력하고 입력 parameter를 확인합니다. 그리고 파일을 열어 Batch mode의 main loop를 실행시킵니다. Batch mode의 main loop에서는 입력 파일을 순차적으로 읽으면서 입력 값을 파일에 저장하고 insert, delete, 혹은 search 함수를 호출합니다. fprintTree함수를 이용하여 결과를 output.txt에 저장합니다.

Interactive mode 부분에서는 Interactive mode 관련 메시지를 출력하고 Interactive mode의 main loop를 실행시킵니다. Interactive mode의 main loop에서는 사용자의 입력을 받고, 입력 값을 출력한 뒤에 insert, delete 또는 search 함수를 호출합니다. 그리고 printTree 함수를 호출하여 Tree를 화면에 출력합니다.

#### makeNode
makeNode 함수는 새로운 node를 만들 때 사용됩니다. node 포인터에 메모리를 할당하고, 그 안에 들어있는 모든 포인터들을 nullptr로 초기화 합니다. 또한 모든 value를 -1로 초기화하고 만들어진 node의 주소를 반환합니다. 

#### insert
insert 함수는 2-4 Tree의 핵심 기능 중 하나로, 값을 Tree에 삽입할 때 사용됩니다. Insert 함수는 node와 value를 input으로 받고, recursive하게 실행됩니다. 
main함수에서 root에 value를 insert하게 되면, insert함수는 leaf node를 만날 때까지 알맞은 위치의 node에 같은 value를 recursive하게 insert합니다. Leaf node 에서는 2-node 혹은 3-node일 경우에, 알맞은 위치에 value를 삽입하고 함수를 끝냅니다.

4-node일 경우에는 makeNode 함수를 이용하여 새로운 node를 만들고, split 함수를 이용해 4-node leaf의 자리에 새로운 2-node를 넣고 그 2-node의 cNode로 3-node와 2-node leaf를 만듭니다. (split함수 파트에서 자세히 설명)

이렇게 되면 insert가 이루어진 node에서만 height가 1 증가하게 됩니다. 그래서 propagate 함수를 이용하여 split으로 만들어진 2-node를 propagation을 통해 pNode와 합쳐지게 합니다. pNode가 4-node일 경우에는, recursive하게 propagate가 실행됩니다. (propagate함수 파트에서 자세히 설명)

#### split
split은 insert나 propagate 도중, 4-node에 2-node를 삽입하려고 할 경우에 호출됩니다. split은 4-node와 2-node를 parameter로 받습니다. 두개의 node안에 있는 4개의 value 중에서 3번째로 작은 value를 찾아 새로운 2-node를 만듭니다. 나머지 value는 3-node와 2node에 각각 2개, 1개씩 담겨서 앞서 만들어진 2-node의 cNode가 됩니다. Split은 특정 node의 depth를 비대칭적으로 증가시킵니다. 따라서 split을 수행한 node는 반드시 propagate 되어야 합니다.

#### propagate
propagate는 2-node를 pNode와 합쳐 하나의 node로 만드는 함수입니다. Propagate는 2-node를 parameter로 받습니다. parameter node의 pNode를 찾아 propagation을 실행하여 두개의 node를 하나의 node로 만듭니다. Parameter 2-node에 있던 하나의 value를 pNode로 옮기고, 두개의 cNode 또한 pNode의 cNode로 옮겨줍니다.

Parameter node의 pNode가 4-node일 경우에는, split과 propagate를 recursive하게 호출합니다.

#### search
search함수는 2-4 Tree의 핵심 기능 중 하나입니다. search함수는 최초로 root node와 찾으려는value를 parameter로 받습니다. 그러면 해당 node (root node)의 value중에 일치하는 value가 있는지 확인합니다.

일치하는 값이 없을 경우, 해당 node의 cNode중에서 찾으려는 value가 들어갈 수 있는 node를 찾아서 recursive하게 search를 실행합니다.

Parameter value와 일치하는 값이 Tree 안에 존재할 경우, 전역변수인 nodeA에 해당 node를 저장하고, 찾은 value의 index를 반환합니다.

찾으려는 value가 존재하지 않을 경우, -1을 반환합니다.

#### delete
delete함수도 2-4 Tree의 핵심 기능 중 하나입니다. delete함수는 root node와 제거하려는 value를parameter로 받습니다. 입력 받은 value를 Tree에서 search한 뒤에, value가 존재할 경우 해당 value를 Tree에서 제거합니다.

delete함수가 호출되면, 가장 먼저 search 함수가 호출되고, 해당 value의 위치를 찾습니다. 찾은 node와 index를 putSuccessor함수에 입력하면, putSuccessor함수는 제거하려는 value의 위치에 해당 value의 successor를 넣습니다. 그리고 Successor가 있던 위치를 nodeA에 저장하고 index를 반환합니다.

이렇게 되면 제거하려는 value는 leaf node에 위치하게 됩니다. 해당 node가 3-node 혹은 4-node일 경우에는 값을 제거하고 delete가 완료됩니다.

해당 node가 2-node일 경우에는 value를 제거할 경우, depth가 비대칭적으로 바뀌게 되므로 이를 해결하기 위하여 reconst함수를 호출합니다. (reconst함수 파트에서 자세히 설명)

reconst함수가 recursive하게 호출되어 2-4 Tree가 정상적인 모양으로 바뀌면 delete 과정은 끝이 납니다.

#### reconst
reconst함수는 delete로 인해 depth가 비대칭적으로 바뀐 Tree를 정상적인 Tree로 만드는 함수입니다. Value가 없어진 node를 parameter로 받습니다. 해당 node의 sibling을 찾아서 sibling중에 3-node 혹은 4-node가 존재할 경우 Transfer 함수를 호출합니다. 3-node나 4-node sibling이 왼쪽에 있을 경우, 오른쪽으로 transfer하는 transferR함수를 호출하고, 오른쪽에 있을 경우 transferL함수를 호출합니다.

Parameter node의 sibling이 모두 2-node일 경우, pNode가 3-node 혹은 4-node인지 체크합니다. 그럴 경우 fusion함수를 호출합니다.

pNode도 2-node일 경우에는 sibling node를 propagate합니다. 그리고 propagate된 node를 recursive하게 reconstruct합니다.

#### transferR
transferR함수는 비어있는 node와 pNode를 parameter로 받아서 transfer를 통해 빈 node를 채워주는 함수입니다.

빈 node의 왼쪽에 있는 node가 giver가 되고 오른쪽 방향으로 transfer하여 빈 node를 채워줍니다. 이때, findNum함수를 이용하여 giver node의 value 개수를 먼저 체크하게 되는데 giver node의 value가 1개였을 경우에는 giver node가 receiver node가 돼서 다시 recursive하게 transferR함수를 호출하여 빈 node가 없도록 만듭니다.

#### transferL
transferL함수는 transferR함수와 동일하지만 방향만 왼쪽으로 바뀐 함수입니다.

#### fusion
fusion 함수는 parameter로 빈 node와 pNode를 받아서 fusion을 수행하는 함수입니다. 먼저 fusion을 오른쪽 sibling과 할지, 왼쪽 sibling과 할지 결정합니다. 그리고 pNode의 알맞은 위치에서 value를 받고 sibling과 합쳐져서 하나의 node가 됩니다.

그리고 fusion이 완료되어 Tree에서 벗어나게 된 node는 free함수로 메모리할당을 해제합니다.

#### equalizeNode
equalizeNode는 parameter node의 포인터들을 올바르게 설정해주는 함수입니다. Propagate를 하면서 cNode들을 여기저기 옮기다 보니 해당 cNode들의 pNode를 바꿔주는 코드를 일일이 작성하기 번거로워서 만든 함수입니다.

equalizeNode함수를 호출하면 해당 node의 cNode들의 pNode를 해당 node로 모두 바꿔줍니다.

#### findNum
findNum함수는 parameter node의 value개수를 찾아주는 함수입니다. 즉 해당 node가 2-node인지, 3-node인지, 4-node인지 찾아줍니다.

findNum함수는 transfer함수에서 호출되어 giver가 2-node인지 판별할 때 사용됩니다.

#### printNode
printNode는 하나의 node를 화면에 출력해주는 코드입니다. Parameter로 받은 node가 2-node, 3-node 혹은 4-node인지 판별하여 해당 모양에 맞게 화면에 value를 출력합니다.

printNode함수는 printTree함수에서 호출되어 전체 Tree를 출력할 때 사용됩니다.

#### printTree
printTree함수는 전체 Tree를 화면에 출력할 때에 사용되는 함수입니다. Parameter로 root node를 입력받고 root node부터 leaf node까지 한 줄에 하나의 depth씩 전체 Tree를 출력합니다.

printTree 함수에는 nodeA배열과 nodeB배열이 있는데, 각각의 배열에 하나의 depth에 있는 모든 node를 저장한 뒤에 printNode함수를 이용하여 해당 배열의 node를 차례로 print합니다.

배열이 print될 때에 나머지 배열에 프린트 되는 배열의 cNode들을 차례로 저장하여 다음 번 루프에서 출력합니다. 이 과정이 Tree의 height만큼 반복됩니다.

#### fprintNode
fprintNode는 printNode와 동일하나 화면에 node를 출력하는 대신에, output.txt파일에 node를 write합니다.

#### fprintTree
fprintTree는 printTree와 동일하나 printNode함수 대신에 fprintNode함수를 호출합니다.

## 교과서 알고리즘과 구현 코드의 비교

#### insert
교과서 알고리즘과 완전히 동일하게 구현하였습니다. 추가로 양의 정수만 insert한다고 가정하였음으로 value의 초기값에 -1을 대입하여 비어 있는 것을 표현하였습니다.

#### delete
delete의 시작부분에 search하여 value가 Tree안에 존재하는 지 먼저 확인하였습니다. 또한 수업자료에는 delete를 할 때에 predecessor와 swap하였는데, 제가 구현한 코드는 과제 안내문에 따라서 successor와 swap합니다.

Reconstruct를 할 때에 sibling node중에 3-node나 4-node가 하나라도 존재할 경우 transfer를 우선적으로 수행하였습니다. 해당 node가 인접한 node가 아닐 경우, 인접한 node부터 차례로 transfer를 여러 번 수행하였습니다.

#### search
교과서 알고리즘과 동일하게 작성하였으나, delete과정에서 search함수를 이용하기 위하여 전역 변수에 찾은 value의 위치를 저장하고 index를 반환하는 형태로 코드를 작성하였습니다.
