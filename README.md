# Bi-gram Analyzer
### 📝 Abstract
- Bi-gram Analyzer 이란 문서에서 발견 가능한 두 단어의 조합으로써, 이를 분석함으로써 문서에 어떤 단어의 빈도가 높은 지 분석할 수 있는 프로그램이다
- Bi-gram Analyzer 의 최적화를 총 6단계에서 진행했는데, 진행과정은 다음과 같다.
  - Sort (Bi-gram의 빈도수를 sort 하는 과정)
  - List (linked list를 scanning 하고, 새로운 element를 추가하는 과정)
  - Lower (string의 대문자를 소문자로 바꾸어 주는 과정)
  - Strlen (문자열의 길이를 재는 과정)
  - Hash (Hash table 내 bucket에 hash function을 사용하는 과정)
  - Rest (나머지 과정)
- GCC내 gprof를 사용하여 총 6가지 과정을 최적화 하였다.
<br>

## 보고서
- [📈 Wrap-Up report]((https://github.com/user-attachments/files/18051858/Bigram.analyzer.pdf)
)
