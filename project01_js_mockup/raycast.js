const TILE_SIZE = 32;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;
const WINDOW_WIDTH = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;
const FOV_ANGLE = 60 * (Math.PI / 180); /* 視線の範囲60°を、ラジアン(π/180)に変換して保持する */
const WALL_STRIP_WIDTH = 1; /* ひとつのrayが壁に当たった場合に、画面上に描画する壁の幅(px)。任意に決められるように定数として定義する */
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH; /* キャストするrayの数は、1本のrayが描画する壁の幅で、画面の横幅を割った値になる */

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
  hasWallAt(x, y){ /* 座標(x, y)に壁が存在しているかどうかを判定する */
    if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT){
      return true;
    }
    var mapGridIntex_x = Math.floor(x / TILE_SIZE); /* Mapクラス内で保持しているgridのインデックスに合わせる */
    var mapGridIntex_y = Math.floor(y / TILE_SIZE); /* 同上。Map座標をタイルの大きさで除算することでindexの値がわかる */
    return (this.grid[mapGridIntex_y][mapGridIntex_x] != 0); /* 壁がある場合は、grid[x][y]の値は1になる。壁以外にもスプライトなどを置くことを想定して、'0ではない'という条件にしている */
  }
  render () {
    for (var i = 0; i < MAP_NUM_ROWS; i++){
      for (var j = 0; j < MAP_NUM_COLS; j++){
        var tileX = j * TILE_SIZE;
        var tileY = i * TILE_SIZE;
        var tileColor = (this.grid[i][j] == 1) ? "#333" : "#fff";
        stroke ("#333");
        fill(tileColor);
        rect(tileX, tileY, TILE_SIZE, TILE_SIZE);
      }
    }
  }
}

class Player {
  constructor() {
    this.x = WINDOW_WIDTH / 2;
    this.y = WINDOW_HEIGHT / 2;
    this.radius = 3;
    this.turnDirection = 0; /* 左に旋回している場合に-1、右に旋回している場合に1 */
    this.walkDirection = 0; /* 前進している場合に1、後退している場合に-1 */
    this.rotationAngle = Math.PI / 2; /* プレイヤーの向き(FOVの中心) */
    this.moveSpeed = 3.0; /* 1フレームで3ピクセル移動する */
    this.rotationSpeed = 3 * (Math.PI / 180); /* 1フレームで3°回転する */
  }
  update(){
    this.rotationAngle += this.turnDirection * this.rotationSpeed;
    var moveStep = this.walkDirection * this.moveSpeed;
    var newPlayerPos_x = this.x + Math.cos(this.rotationAngle) * moveStep;
    var newPlayerPos_y = this.y + Math.sin(this.rotationAngle) * moveStep;

    if (!grid.hasWallAt(newPlayerPos_x, newPlayerPos_y)){ /* 移動した先に壁がない場合のみ、移動させる */
      this.x = newPlayerPos_x;
      this.y = newPlayerPos_y;
    }

  }
  render(){
    noStroke();
    fill("#f00");
    circle(this.x, this.y, this.radius);
    stroke("#f00");
    line(this.x, this.y, this.x + Math.cos(this.rotationAngle) * 20, this.y + Math.sin(this.rotationAngle) * 20);
  }
}

/* ここまでやった */

class Ray{
  constructor(rayAngle){
    this.rayAngle = rayAngle;
  }
  render(){
    stroke("rgba(255, 0, 0, 0, 0.3)");
    line( /* playerの現在位置から、rayAngle の方向に30px動かした座標まで直線を描画する */
      player.x,
      player.y,
      player.x + Math.cos(this.rayAngle) * 30, 
      player.y + Math.sin(this.rayAngle) * 30  
    );
  }
}

var grid = new Map();
var player = new Player();
var rays = []; /* rayを格納しておく配列 */

function keyPressed(){
  if (keyCode  == UP_ARROW){
    player.walkDirection = +1;
  }else if (keyCode == DOWN_ARROW){
    player.walkDirection = -1;
  }else if(keyCode == RIGHT_ARROW){
    player.turnDirection = +1;
  }else if(keyCode == LEFT_ARROW){
    player.turnDirection = -1;
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
** castAllRays():
** Rayクラスからインスタンスを作成し、壁に当たるかを確認する
 */
function castAllRays(){
  var colmnId = 0;
  var rayAngle = player.rotationAngle - (FOV / 2); /* 最初のrayの角度は、プレイヤーの向きからFOVの半分を引いた値(プレイヤーの向きは、FOVの中心なので) */
  rays = [];

  for (var i=0; i<NUM_RAYS; i++){
    var ray = new Ray(rayAngle);
    rays.push(ray);
    rayAngle += FOV_ANGLE / NUM_RAYS; /* 次のrayの角度は、rayの数でFOVを除算した値だけ変化する */
    columnId++;
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
  castAllRays();
}

/*
 ** draw():
 ** フレーム更新ごとに画面の描画を行う
 */
function draw() {
  update();
  grid.render();
  for (ray of Rays){ /* 保持したrayを1本ずつ描画する */
    ray.render();
  }
  player.render();
}