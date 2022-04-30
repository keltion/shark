# shark browser
Toy Browser Project for Understanding Browser Engines. 
I implement a simple version of the browser in this project.

## DOM Tree
Currently, there are only two Nodes of Dom, Element and Text.

## HTML Parser
The HTML parser in this project is very strict.
1. No unnecessary spaces are allowed.
    ex)
    ```html
    <html > <!--(x)-->
    <html> <!--(o)-->
    ```
    ```html
    <div id ="div1"> <!--(x)-->
    <div id="div"> <!--(o)-->
    ```

2. Element omission is not allowed.
3. It does not allow omission of closed tags.
---
<br>  

## Implemented so far
Tokenizes elements in HTML like the following (character is not tokenized yet).
```html
<html><head></head><body><div id="div1"></div><div id="div2"></div></body></html>
``` 
![image](https://user-images.githubusercontent.com/76467273/161520929-c8e57b5c-fbf1-4391-b9fc-eada735ce4e1.png)

## Plan
1. Implementing character tokenization.
2. Logic to create each token as an Element or Text object according to the type of token.
3. Implementation of HTMLTreeBuilder that creates a DOM tree based on the object created in step 2.
4. Implement a simple version of the css parser.