# shark browser
브라우저 엔진을 이해하기 위한 토이 브라우저 프로젝트

## DOM Tree
현재 Dom의 Node는 Element와 Text 두가지만 존재합니다.

## HTML Parser
이 프로젝트에서의 HTML parser는 굉장히 엄격합니다.
1. 쓸데 없는 띄어쓰기를 허용하지 않습니다.  
    ex)
    ```html
    <html > <!--(x)-->
    <html> <!--(o)-->
    ```
    ```html
    <div id ="div1"> <!--(x)-->
    <div id="div"> <!--(o)-->
    ```

2. Element의 생략을 허용하지 않습니다.
3. 닫힌 태그 생략을 허용하지 않습니다.
---

## 현재까지 구현된 부분
다음과 같은 HTML을 각 태그별로 토큰화하는 작업까지 구현하였습니다. 아직 character는 토큰화하지 못합니다.
```html
<html><head></head><body><div id="div1"></div><div id="div2"></div></body></html>
``` 
## 구현 결과
![image](https://user-images.githubusercontent.com/76467273/161520929-c8e57b5c-fbf1-4391-b9fc-eada735ce4e1.png)

## 계획
1. character 토큰화 구현
2. 각 토큰을 토큰의 Type에 따라 Element 또는 Text 객체로 생성하는 로직구현
3. 2에서 만들어진 객체를 바탕으로 DOM Tree를 생성하는 HTMLTreeBuilder 구현
4. 간단한 버전의 css parser 구현