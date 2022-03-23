var maxFood = 5;
var randomMax = 0;
var foods = new Array(0);
var foodsX = new Array(0);
var foodsY = new Array(0);
var bodys = new Array(0);
var component;
var moveDir = Qt.Key_Up;
var preMoveDir = Qt.Key_Up;
var gameOver = false;

function getNewFoodXY(cellWidth, cellHeight){
    var r,x,y;
    if(foods.length==0){
        r = Math.random();
        x = Math.floor(r*play.width);
        y = Math.floor(r*play.height);
        foodsX.push(x);
        foodsY.push(y);
        return;
    }

    var i=0;
    while(i<foods.length){
        for(i=0; i<foods.length; i++){
            r = Math.random();
            x = Math.floor(r*play.width);
            y = Math.floor(r*play.height);

            if(x>=foodsX[i]-cellWidth && x<=foodsX[i]+cellWidth){
                if(y>=foodsY[i]-cellHeight && y<=foodsY[i]+cellHeight){
                    break;
                }
            }
        }
    }

    foodsX.push(x);
    foodsY.push(y);
}

function producer(){
    if (component == null)
        component = Qt.createComponent("Cell.qml");

    // Note that if Block.qml was not a local file, component.status would be
    // Loading and we should wait for the component's statusChanged() signal to
    // know when the file is downloaded and ready before calling createObject().
    if (component.status == Component.Ready) {
        var dynamicObject = component.createObject(play);
        if (dynamicObject == null) {
            console.log("error creating block");
            console.log(component.errorString());
            return null;
        }else{
            return dynamicObject;
        }
    } else {
        console.log("error loading block component");
        console.log(component.errorString());
        return null;
    }
}

function foodProducer(){
    if(randomMax == 0 || foods.length == 0){
        var r = Math.random();
        randomMax = Math.floor(r*maxFood);
    }

    if(foods.length >= randomMax){
        return true;
    }

    var dynamicObject = producer();
    if (dynamicObject == null) {
        return false;
    }

    getNewFoodXY(dynamicObject.width, dynamicObject.height);
    var x = foodsX[foodsX.length-1];
    var y = foodsY[foodsY.length-1];
    console.debug('food:',x, y);
    dynamicObject.x = x;
    dynamicObject.y = y;
    foods.push(dynamicObject);
}

function bodyInit(){
    var x = play.width/2;
    var y = play.height/2;

    for(var i=0; i<5; i++){
        var dynamicObject = producer();
        dynamicObject.x = x;
        dynamicObject.y = y;
        if(i==0){
            dynamicObject.color = 'red';
        }else{
            dynamicObject.color = 'white';
        }
        bodys.push(dynamicObject);
        y += dynamicObject.height+1;
    }
}

function newGame(){
    play.score = 0;
    play.level = 1;
    gameOver = false
    moveDir = Qt.Key_Up;
    preMoveDir = moveDir;

    var temp;
    while(bodys.length > 0){
        temp = bodys.pop();
        temp.destroy();
    }
    while(foods.length > 0){
        temp = foods.pop();
        temp.destroy();
    }
    while(foodsX.length > 0){
        foodsX.pop();
    }
    while(foodsY.length > 0){
        foodsY.pop();
    }

    bodyInit();
}

function changeDir(dir){
    preMoveDir = moveDir;
    moveDir = dir;
}

function move(){
    var head = bodys[0]
    var preX = head.x;
    var preY = head.y;
    var moveStep;

    var bAgain;
    var x,y;
    do{
        bAgain = false;
        x = head.x;
        y = head.y;

        switch(moveDir){
        case Qt.Key_Up:
            moveStep = head.height+1;
            y -= moveStep;
        break;

        case Qt.Key_Down:
            moveStep = head.height+1;
            y += moveStep;
        break;

        case Qt.Key_Right:
            moveStep = head.width+1;
            x += moveStep;
        break;

        case Qt.Key_Left:
            moveStep = head.width+1;
            x -= moveStep;
        break;
        default:
            return;
        }

        //头部移动后马上与下一点重叠，不允许移动
        if(x>=bodys[1].x-head.width && x<=bodys[1].x+head.width){
            if(y>=bodys[1].y-head.height && y<=bodys[1].y+head.height){
                console.debug('cannot move this dir,continu pre dir')
                moveDir = preMoveDir;
                bAgain = true;
            }
        }
    }while(bAgain);

    head.x = x;
    head.y = y;
    var tempX,tempY;
    for(var i=1; i<bodys.length; i++){
        tempX = bodys[i].x;
        tempY = bodys[i].y;
        bodys[i].x = preX;
        bodys[i].y = preY;
        preX = tempX;
        preY = tempY;
    }

    checkDie();
    checkEatFood();
}

function autoMove(){
    move()
}

function checkGameOver(){
    return gameOver;
}

function checkEatFood(){
    var head = bodys[0];

    var i;
    for(i=0; i<foods.length; i++){
        if(head.x>=foodsX[i]-head.width && head.x<=foodsX[i]+head.width){
            if(head.y>=foodsY[i]-head.height && head.y<=foodsY[i]+head.height){
                break;
            }
        }
    }

    if(i<foods.length){
        //eat food
        var foodEat = foods[i];
        console.debug('eat:'+foodsX[i]+","+foodsY[i])

        if(moveDir == Qt.Key_Up){
            foodEat.x = head.x;
            foodEat.y = head.y - head.hight - 1;
        }else if(moveDir == Qt.Key_Down){
            foodEat.x = head.x;
            foodEat.y = head.y + head.hight + 1;
        }else if(moveDir == Qt.Key_Left){
            foodEat.y = head.y;
            foodEat.x = head.x - head.width - 1;
        }else if(moveDir == Qt.Key_Right){
            foodEat.y = head.y;
            foodEat.x = head.x + head.width + 1;
        }

        foodEat.color = 'red';
        head.color = 'white';
        bodys.unshift(foodEat);
        for(var j=i; j<foods.length-1; j++){
            foods[j] = foods[j+1]
            foodsX[j] = foodsX[j+1]
            foodsY[j] = foodsY[j+1]
        }
        foods.pop();
        foodsX.pop();
        foodsY.pop();

        play.score += 1

        updateLevel();
    }
}

function checkDie(){
    var head = bodys[0];

    if(head.x < 0 || head.x+head.width > play.width){
        console.info('die for x out range!')
        gameOver = true;
        return;
    }

    if(head.y < 0 || head.y+head.height > play.height){
        console.info('die for y out range!')
        gameOver = true;
        return;
    }

    for(var i=1; i<bodys.length; i++){
        if(head.x>=bodys[i].x-bodys[i].width && head.x<=bodys[i].x+bodys[i].width){
            if(head.y>=bodys[i].y-bodys[i].height && head.y<=bodys[i].y+bodys[i].height){
                console.info('die for self hit!')
                bodys[i].color = 'yellow';
                gameOver = true;
                return;
            }
        }
    }
}

function updateLevel(){
    if(play.score % 2 == 0){
        play.level += 1;
    }

    if(timer_produce_food.interval > 100){
        timer_produce_food.interval -= 100;
    }
}
