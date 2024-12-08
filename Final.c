#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hashtable 노드 구조체 정의
typedef struct Node {
  char key[100];      // bigram string을 저장하는 배열
  int value;         // bigram frequency
  struct Node *next; // 다음 노드를 가리키는 포인터
} Node;

// Hashtable 구조체 정의
typedef struct Hashtable {
  int size;       // 해시 테이블의 크기
  Node **buckets; // 버킷 배열
} Hashtable;

void lower2(char *s) {
  long i;
  long len = strlen(s);
  for (i = 0; i < len ; i++)
    if (s[i] >= 'A' && s[i] <= 'Z')
      s[i] -= ('A' - 'a');
}


// Hashtable을 초기화하는 함수
Hashtable *initializeHashtable(int size) {
  Hashtable *hashtable = (Hashtable *)malloc(sizeof(Hashtable));
  if (hashtable == NULL) {
    fprintf(stderr, "메모리 할당 오류\n");
    exit(EXIT_FAILURE);
  }

  // 버킷 배열 초기화
  hashtable->buckets = (Node **)malloc(size * sizeof(Node *));
  if (hashtable->buckets == NULL) {
    fprintf(stderr, "메모리 할당 오류\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < size; ++i) {
    hashtable->buckets[i] = NULL;
  }

  hashtable->size = size;

  return hashtable;
}

// 해시 함수: key를 해시 테이블의 크기로 나눈 나머지를 반환
unsigned int DJB2(const char *key, int size) {
  unsigned int hashVal = 5381;
  char c;
  while((c=*key++))
  {
    hashVal = (((hashVal<<5)+hashVal)+c) % size ;
  }
  return hashVal % size;
}


unsigned int hash(const char *key, int size) { return DJB2(key, size); }

void mergeSort(Node **head);
void splitList(Node *source, Node **frontRef, Node **backRef);
Node *mergeLists(Node *a, Node *b);

void mergeSort(Node **head) {
  Node *tempHead = *head;
  Node *front;
  Node *back;

  // Base case: If the list is empty or has only one element, it's already
  // sorted
  if (tempHead == NULL || tempHead->next == NULL) {
    return;
  }

  // Split the list into two halves
  splitList(tempHead, &front, &back);

  // Recursively sort the two halves
  mergeSort(&front);
  mergeSort(&back);

  // Merge the sorted halves
  *head = mergeLists(front, back);
}

void splitList(Node *source, Node **frontRef, Node **backRef) {
  Node *slow;
  Node *fast;

  if (source == NULL || source->next == NULL) {
    *frontRef = source;
    *backRef = NULL;
  } else {
    slow = source;
    fast = source->next;

    // Move fast two nodes and slow one node
    while (fast != NULL) {
      fast = fast->next;
      if (fast != NULL) {
        slow = slow->next;
        fast = fast->next;
      }
    }

    // 'slow' is before the midpoint, split the list into two at that point
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}
Node *mergeLists(Node *a, Node *b) {
  Node *result = NULL;

  // Base cases: If either list is empty, return the other
  if (a == NULL) {
    return b;
  } else if (b == NULL) {
    return a;
  }

  // Compare the data of the two lists and merge them in ascending order
  if (a->value >= b->value) {
    result = a;
    result->next = mergeLists(a->next, b);
  } else {
    result = b;
    result->next = mergeLists(a, b->next);
  }

  return result;
}

void insertNode(Hashtable *hashtable, const char *key) {
  // 해시 함수를 사용하여 버킷을 선택
  unsigned int index = hash(key, hashtable->size);

  // 중복된 bigram 탐지를 위한 변수
  Node *current = hashtable->buckets[index];
  Node *prev = NULL;

  // 연결 리스트의 끝까지 이동하여 새로운 노드 삽입 또는 중복된 bigram 찾기
  while (current != NULL) {
    // 이미 존재하는 bigram인 경우 frequency 증가하고 종료
    if (strcmp(current->key, key) == 0) {
      current->value += 1;

      // 삽입 후에 정렬하는 코드 추가
      mergeSort(&hashtable->buckets[index]);

      return;
    }

    // 다음 노드로 이동
    prev = current;
    current = current->next;
  }

  // 중복된 bigram이 없는 경우, 새로운 노드 생성
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (newNode == NULL) {
    fprintf(stderr, "메모리 할당 오류\n");
    exit(EXIT_FAILURE);
  }

  // 새로운 노드에 데이터 설정
  strcpy(newNode->key, key);
  newNode->value = 1;
  newNode->next = NULL;

  // 선택된 버킷에 노드 삽입
  if (hashtable->buckets[index] == NULL) {
    hashtable->buckets[index] = newNode;
  } else {
    // 노드를 삽입한 후에 해당 버킷 내의 노드들을 insertion sort로 정렬
    if (prev == NULL) {
      newNode->next = hashtable->buckets[index];
      hashtable->buckets[index] = newNode;
    } else {
      newNode->next = current;
      prev->next = newNode;
    }
    mergeSort(&hashtable->buckets[index]);
  }
}
void extractBigramAndInsert(Hashtable *hashtable, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "파일을 열 수 없습니다.\n");
    exit(EXIT_FAILURE);
  }

  char word1[50], word2[50];

  if (fscanf(file, "%s", word1) == 1) {
    lower2(word1);
    while (fscanf(file, "%s", word2) == 1) {
      lower2(word2);
      char bigram[100];
      sprintf(bigram, "%s %s", word1, word2);

      unsigned int index = hash(bigram, hashtable->size);

      // 해당 버킷에 bigram 삽입
      insertNode(hashtable, bigram);

      strcpy(word1, word2);
    }
  }

  fclose(file);
}

// Hashtable 내의 노드를 출력하는 함수
void printHashtable(Hashtable *hashtable) {
  for (int i = 0; i < hashtable->size; ++i) {
    printf("Bucket %d:\n", i);
    Node *current = hashtable->buckets[i];
    while (current != NULL) {
      printf(" Key: %s / Value: %d\n", current->key, current->value);
      current = current->next;
    }
    printf("\n");
  }
}

// Hashtable 내의 노드를 메모리 해제하는 함수
void freeHashtable(Hashtable *hashtable) {
  for (int i = 0; i < hashtable->size; ++i) {
    Node *current = hashtable->buckets[i];
    while (current != NULL) {
      Node *next = current->next;
      free(current);
      current = next;
    }
  }
  free(hashtable->buckets);
  free(hashtable);
}

void countEmptyBuckets(Hashtable *hashtable) {
  int count = 0;

  for (int i = 0; i < hashtable->size; ++i) {
    if (hashtable->buckets[i] == NULL) {
      count++;
    }
  }
  printf("%d", count);
}
int main() {
  const char *filename = "text_file.txt";
  int size = 20000; // size of bucket
  Hashtable *hashtable = initializeHashtable(size);

  extractBigramAndInsert(hashtable, filename);

  // Hashtable 내의 노드 출력
  printHashtable(hashtable);
  countEmptyBuckets(hashtable);
  // Hashtable 내의 노드 메모리 해제
  freeHashtable(hashtable);

  return 0;
}

