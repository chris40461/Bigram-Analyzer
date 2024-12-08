# Bigram-Analyzer


Bigram Analyzer 이란 문서에서 발견 가능한 두 단어의 조합으로써, 이를 분석함으로써 문서에 어떤 단어의 빈도가 높은 지 분석할 수 있는 프로그램을 뜻한다. 이 프로그램을 통해서 소위 말하는 ‘알고리즘’을 분석할 수 있는데, 이는 곧 실시간 검색어의 분석으로도 쓰일 수 있을 것이다. Bigram Analyzer 은 크게 Sort (Bigram의 빈도수를 sort 하는 과정), List (linked list를 scanning 하고, 새로운 element를 추가하는 과정), Lower (string의 대문자를 소문자로 바꾸어 주는 과정) Strlen (문자열의 길이를 재는 과정), Hash (Hash table 내 bucket에 hash function을 사용하는 과정), Rest (나머지 과정)으로 나눌 수 있다. GCC내 gprof를 사용하여 총 6가지 과정의 runtime을 측정할 것이며, 측정된 runtime을 통해 어떤 과정의 runtime 이 최적화가 필요한지 분석하고 최적화하는 과정을 진행하게 될 것이다.
