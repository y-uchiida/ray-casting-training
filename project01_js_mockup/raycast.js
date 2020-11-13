const TILE_SIZE = 32;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;
const WINDOW_WIDTH = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

class Map {
  constructor() {
    this.grid = [
      [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
      [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
      [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
      [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
      [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
    ];
  }
  hasWallAt(x, y){
    if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT){
      return true;
    }
    var mapGridIntex_x = Math.floor(x / TILE_SIZE);
    var mapGridIntex_y = Math.floor(y / TILE_SIZE);
    return (this.grid[mapGridIntex_y][mapGridIntex_x] != 0);
  }
  render () {
    for (var i = 0; i < MAP_NUM_ROWS; i++){
      var tileX = j * TILE_SIZE;
      var tileY = i * TILE_SIZE;
      var tileColor = (this.grid =[i][j] == 1) ? "#333" : "#fff";
      stroke ("#333");
      fill(tileColor);
      rect(tileX, tileY, TILE_SIZE, TILE_SIZE);
    }
  }
}
class Player {
  constructor() {
    this.x = WINDOW_HEIGHT / 2;
    this.y = WINDOW_WIDTH / 2;
    this = radius = 3;
    this.turnDirection = 0;
    this.walkDirection = 0;
    this.rotationAngle = Math.PI / 2;
    this.moveSpeed = 3.0; // 1フレームで3ピクセル移動する
    this.rotationSpeed = 3 * (Math.PI / 180); // 1フレームで3°回転する
  }
  update()
  {
    this.rotationAngle += this.turnDirection * this.rotationSpeed;
    var moveStep = this.walkDirection * this.moveSpeed;
    var newPlayerPos_x = this.x + Math.cos(this.rotationAngle) * moveStep;
    var newPlayerPos_y = this.y + Math.sin(this.rotationAngle) * moveStep;

    if (!grid.hasWallAt(newPlayerPos_x, newPlayerPos_y)){
      this.x = newPlayerPos_x;
      this.y = newPlayerPos_y;
    }

  }
  render()
  {
    noStroke();
    fill("#f00");
    circle(this.x, this.y, this.radius);
    stroke("#f00");
    line(this.x, this.y, this.x + Math.cos(this.roationAngle) * 20, this.y + Math.sin(this.rotationAngle) * 20);
  }
}

var grid = new Map();
var player = new Player();

function keyPressed(){
  if (keyCode  == UP_ARROW){
    player.walkDirection = +1;
  }else if (keyCode == DOWN_ARROW){
    player.walkDirection = -1;
  }else if(keyCode == RIGHT_ARROW){
    player.turnDirection = +1;
  }else if(keyCode == LEFT_ARROW){
    plauer.turnDirection == -1;
  }
}

function keyReleased(){
  if (keyCode  == UP_ARROW || keyCode == DOWN_ARROW){
    player.walkDirection = 0;
  }else if(keyCode == RIGHT_ARROW || keyCode == LEFT_ARROW){
    player.turnDirection = 0;
  }
}

/*
 ** setup():
 ** オブジェクトおよびデータの初期化を行う
 */
function setup() {
  createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

/*
 ** update():
 ** 入力内容などに応じて、オブジェクトの内容を更新する
 */
function update() {
  player.update();
}

/*
 ** draw():
 ** フレーム更新ごとに画面の描画を行う
 */
function draw() {
  update();
  grid.render();
  player.render();
}