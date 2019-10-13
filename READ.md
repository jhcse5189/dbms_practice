# **DBMS_ITE2038_13130**

>### **1. Possible call path of the insert/delete operation**
* ## ***Insertion***
\
In main

    case 'i':
        scanf("%d", &input);
        root = insert(root, input, input);
        print_tree(root);
        break;

In bpt.c ( line 456 ~ 848 )

    (master insertion func.) line 803 ~ 848



1. 먼저 main에서, root에 key-value에 해당하는 input을 parameter로
master insertion function에 전달한다.

    
    >node * insert( node * root, int key, int value );
    

2. 트리에 동일한 key가 있는지
    
    >record * find( node * root, int key, bool verbose );
    
    를 호출하여 확인한다. 동일한 key가 존재하면 main으로 root를 그대로 반환하여 insertion을 중단한다. find가 NULL을 반환할 경우 insert를 진행한다.

3. 해당 value를 동적할당으로 갖는 record를 만들기 위해

    >record * make_record( int value );

    를 호출하여 pointer라는 이름에 할당한다.

* **Case 1 :: the tree does not exist,**\
  **-> Start a new tree.**

    트리가 삭제되었거나 아직 존재하지 않아서 root가 NULL인 경우,

    >node * start_new_tree(int key, record * pointer);
    
    를 호출하여 pointer를 root 노드로 삼는 새로운 트리를 만든다.

        node * make_leaf( void );
        node * make_node( void );

        새로운 트리의 root 노드는 key값을 keys 배열 0번째 칸에 담고, pointer는 pointers 배열 0번째 칸에 담긴다. 그리고 pointers 배열 order - 1번째 칸에 NULL을 할당하여, 아직 다음의 이웃 노드를 가리키는 링크가 없음을 말해준다. root의 parent는 NULL이고, make_leaf, make_node의 호출을 통해 num_keys은 초기값 0에서 1로 증가하며 is_leaf는 true이므로 root는 leaf이다. insertion이 종료되고 root는 insert, main으로 반환된다.


* **Case 2 :: the tree already exists.**
\
\
    트리가 이미 존재하는 경우, insert에서 선언한 leaf에
    >node * find_leaf( node * root, int key, bool verbose );

    를 호출하여 반환된 leaf 노드를 받는다. root가 leaf라면, 바로 root를 리턴한다. root가 leaf가 아니라면, i는 0부터 num_keys까지 insert할 key와 i 인덱스에 해당하는 keys 배열 값과 비교하면서, key가 같거나 크다면 i를 증가시키고, 작다면 break에 의해 while block을 탈출하여 i에 해당하는 pointers값을 node *로 casting하여 임시로 저장하고 leaf일 때까지 while의 is_leaf 조건을 다시 확인한다.

* **Case 2-1 :: leaf has room for key and pointer.**
\
\
    leaf에 order - 1 보다 적은 수의 key가 존재할 때, 
    >node * insert_into_leaf( node * leaf, int key, record * pointer );\
    \
    -> insert할 key와 keys 배열과 비교하여 insertion point를 찾은 후, 위치에 맞게 배열을 옮기는 작업을 수행하고 key와 pointer값을 할당한 다음 leaf를 리턴한다.

    를 호출하고 insert가 완료된 root를 main으로 반환한다. 

* **Case 2-2 :: leaf must be split.**
\
\
    leaf에 이미 order - 1 개수 만큼의 key가 존재할 때,\
    split한 이후 leaf에 insertion을 수행해야한다.

* ## ***Deletion***
\
In main

    case 'd':
        scanf("%d", &input);
        root = delete(root, input);
        print_tree(root);
        break;

In bpt.c ( line 853 ~ 1213 )

    (master deletion func.) line 1201 ~ 1213


1. 먼저 main에서, root와 삭제할 key에 해당하는 input을 parameter로 master deletion function에 전달한다. 

2. find를 호출하여 delete할 key가 있는 record를 key_record 노드에 전달하고, find_leaf를 호출하여 delete할 key를 포함하는 leaf 노드를 key_leaf에 전달한다.
    
        key_record, key_leaf 순으로 && operator의 short circuit 방식으로 NULL인지 검사한다. false일 경우 deletion을 중단하고 root를 main으로 반환한다.


        둘 다 NULL이 아닐 경우,
        node * delete_entry( node * root, node * n, int key, void * pointer );
        
        를 호출하여 delete를 수행한다. delete 후에는 key_record를 free하고 main으로 변경된 root를 반환한다.

3. delete_entry는 root, key_leaf (equals 'n'), key, key_record (equals 'pointer') 를 parameter로 받고,

    >node * remove_entry_from_node(node * n, int key, node * pointer);

    를 호출하여 key-pointer pair를 노드 n에서 dele한다.


* **Case 1 :: deletion from the root.**
\
\
    root에서 deletion이 일어난 경우,
    >node * adjust_root(node * root);

    를 호출하고 root를 delete_entry로 반환한다.

        삭제 후, empty 하지않다면 root를 delete_entry로 반환한다.

        만약 root가 empty 하다면 다시 두가지 분기로 나뉜다,


            (1). 자식 노드가 있다면,
                 pointers[0]이 가리키는 자식 노드를 새로운 root로 삼는다.
            
            (2). 자식 노드가 없다면,
                 root가 leaf인 동시에 empty하므로 모든 tree가 empty
                 하기 때문에, adjust_root에서 선언한 root를 NULL로 만든다.

    empty root에 대한 작업 후, 메모리를 free해주고 root를 delete_entry로 반환한다.


* **Case 2 :: deletion from a node below the root.**
\
\
    root 외 deletion으로 internal node나 leaf에서 deletion이 일어난 경우,
    >min_keys = n->is_leaf ? cut(order - 1) : cut(order) - 1;

    internal node와 leaf냐에 따라 삭제가 일어난 노드가 유지될 수 있는 
    minimum한 key 개수를 min_keys에 할당한다.


        (1). 노드에 들어가있는 key의 개수가 min_keys 조건에 맞다면,
             delete로 root를 반환한다.
       
        (2). 최소 사이즈 미만으로 내려가면,
             merge를 위해 coalescence나 redistribution을 수행한다.


>### **2. Detail flow of the structure modification (split, merge)**

* ## **insertion after split**

    >node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer);

    1. 노드의 오른쪽으로 split될 새로운 new_leaf 노드를 make_leaf를 통해 할당받고,\
    임시로 저장할 tmp_keys, tmp_pointers 배열을 선언하고 동적할당 받는다.

    2. insertion_index 값을 찾고, insert 자리를 남겨두고,\
    leaf에서 tmp로 key-pointer pair를 복사하고\
    해당 자리에 key-pointer pair를 insert 한다.

    3. 원래 leaf의 num_keys값을 0으로 두고,\
    split 기준까지 tmp에서 leaf로, tmp에서 new_leaf로 값을 복사하며\
    각각의 시행마다 num_keys값도 증가시킨다.

    4. tmp로부터의 복사가 종료되면 tmp_keys, tmp_pointers를 free 한다.

    5. leaf, new_leaf 노드가 적절한 이웃 노드를 링크하도록 pointers[order - 1] 값을 변경한다.

    6. 복사 이후, 비어있는 각각의 pointers 배열에 NULL을 할당한다.

    7. new_leaf의 부모를 leaf와 같게 한다.

    8. split 결과로 parent 노드의 새로운 키가 될 new_key를 new_leaf의 left most key로 설정하고,
        
        >node * insert_into_parent(node * root, node * left, int key, node * right);

        를 호출하여 다시 insert와 split 과정을 거친다.

        


* ## **merge after deletion**

* **Case 1 :: Coalescence.**

    >node * coalesce_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime);


    1. n이 left most node라서 이웃 노드가 바로 오른쪽 노드로 설정될 때,\
    서로 노드 위치를 swap 하여, 오른쪽에서 왼쪽으로 Coalescence가 일어나도록 한다.

            n이 leaf일 때,
            n의 key-pointer pair들을 insertion_index부터 넣고
            n의 pointers[order - 1]도 옮겨준다.

            n이 leaf가 아닐 때,
            또한 마찬가지이다. 


    2. n과 parent가 공유하는 key를 delete하고 다시 merge 조건을 재귀적으로 만족시킨다.


         먼저 delete에서 key_leaf로 전달받은 노드 n을 get_neighbor_index()로
         전달하여 neighbor_index를 받는다. 같은 부모 노드 기준으로, 왼쪽 칭(?)구가 
         있다면 그 아이의 인덱스를 받는다. 만약 없다면, -1을 받는다.

         neighbor_index에 따라서 k_prime_index를 결정하는데, -1일 경우 0, 그렇지
         않을 경우 neighbor_index와 같게 된다.

         ? 아무튼 지우고 그.. 머.. 2개를 뭐 한다음에
         delete_entry를 아무튼 빨리 끝냈어요 ㅇㅋ?
         root가 수정되고
         delete가 완료 되었으므로, key_record를 free하고 root를 메인으로 리턴한다.

* **Case 2 :: Redistribution.**

    >node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime_index, int k_prime);

    1. n의 key-pointers 배열의 값을 한 칸씩 오른쪽으로 민다.

    2. 이웃 노드의 right end의 key-pointer pair를\
    n의 left end에 넣는다.

    3. k_prime_index에 해당하는 n의 부모 노드의 key를\
    n의 left end key로 변경한다.


>### **3. (Naïve) designs or required changes for building on-disk b+ tree**


* ## Page
> Header

    - Number of records
    - Free space
    - (maybe) a next/last pointer
    - Bitmaps, Slot Table ... 등등

>layouts


    - Record length (fixed or variable)
    - page packing method (packed or unpacked)

각 노드를 sequencial한 Page로 구현하여, 4096 Bytes 씩 노드를 insert 할 때마다,\
할당해준다. page Header는 metadata를 포함하게하고 free, leaf, internal page\
각각의 역할을 수행하도록 구현한다.

* ## Record

    record를 record id ( PageId, Location in Page )를 통해 찾을 수 있도록\
    bpt.h의 record struct를 변경하고 key-value 각각 8, 120바이트로 총 128 Bytes 씩 record가 추가될 때마다 할당해준다. 또한 record length는 Fixed 나 Variable 할 수 있으므로, VARCHAR와 같은 Variable filed는 메모리상 Fixed filed의 끝부터 채워지도록 구현한다.


---
* * *

### **Milestone 1**

➢ Analyze the given b+ tree code and submit a report to the hconnect Wiki.

➢ Your report should includes
1. Possible call path of the insert/delete operation
2. Detail flow of the structure modification (split, merge)
3. (Naïve) designs or required changes for building on-disk b+ tree

* * *
### **Milestone 2**

➢ Implement on-disk b+ tree and submit a report(Wiki) including your design.
* * *
---