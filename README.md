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