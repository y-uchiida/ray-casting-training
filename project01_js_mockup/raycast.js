const TILE_SIZE = 60;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;
const WINDOW_WIDTH = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;
const FOV_ANGLE = 60 * (Math.PI / 180); /* 視線の範囲60°を、ラジアン(π/180)に変換して保持する */
const WALL_STRIP_WIDTH = 4; /* ひとつのrayが壁に当たった場合に、画面上に描画する壁の幅(px)。任意に決められるように定数として定義する */
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH; /* キャストするrayの数は、1本のrayが描画する壁の幅で、画面の横幅を割った値になる */
const MINIMAP_SCALE_FACTOR = 0.2; /* 画面の端に小さいマップを表示する */

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
        rect(
          tileX * MINIMAP_SCALE_FACTOR,
          tileY * MINIMAP_SCALE_FACTOR,
          TILE_SIZE * MINIMAP_SCALE_FACTOR,
          TILE_SIZE * MINIMAP_SCALE_FACTOR
        );
      }
    }
  }
}

class Player {
  constructor() {
    this.x = WINDOW_WIDTH / 2; /* 初期値はマップの中央に配置 */
    this.y = WINDOW_HEIGHT / 2; /* 初期値はマップの中央に配置 */
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
    circle(
      this.x * MINIMAP_SCALE_FACTOR,
      this.y * MINIMAP_SCALE_FACTOR,
      this.radius * MINIMAP_SCALE_FACTOR
    );
    stroke("#f00"); /* プレイヤーの向いている方向に直線を引く */
    line(
      this.x * MINIMAP_SCALE_FACTOR,
      this.y * MINIMAP_SCALE_FACTOR,
      (this.x + Math.cos(this.rotationAngle) * 20) * MINIMAP_SCALE_FACTOR,
      (this.y + Math.sin(this.rotationAngle) * 20) * MINIMAP_SCALE_FACTOR
    );
  }
}

class Ray{
  constructor(rayAngle){
    this.rayAngle = normalizeAngle(rayAngle); /* rayAngleの値を 0 ~ 2πの範囲になるように変換する */
    this.wallHitX = 0; /* 壁に衝突するX座標 */
    this.wallHitY = 0; /* 壁に衝突するY座標 */
    this.distance = 0; /* プレイヤーとrayが衝突した壁との距離 */
    this.wasHitVertical = false;

    /* プレイヤーの向きを保持する */
    /* ※要注意※ */
    /* プログラミングにおいてのラジアンは、右向きから始まり、右回り(時計回り)で計算される！！ */
    /* 0からインクリメントすると、まずは右下の領域を通っていき、次に左下->左上->右上 の順番で動いていく */
    this.isRayFacingDown = (0 < this.rayAngle  && this.rayAngle < Math.PI); /* rayが下向きのとき => 0からπまでの範囲 */
    this.isRayFacingRight = (this.rayAngle < 1/2 * Math.PI || this.rayAngle > 3/2 * Math.PI); /* rayが右向きのとき => 右下の領域にあるとき(Θ < π/2)、または右上の領域にあるとき(3π/2 < Θ) */
  }

  cast(){
    var xintercept, yintercept; /* rayとgridの交点におけるxとyの座標 */
    var xstep, ystep; /* 次のgridとの交点までの長さ(px) */

  //   /* 1. グリッドの横罫線とrayの交点での、壁との距離を計算する */
    var foundHorzWallHit = false;
    var horzWallHitX = 0;
    var horzWallHitY = 0;

  //   /* プレイヤーの座標と向きから、最も近い横罫線との交点のy座標を取得する */
    yintercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE; /* TILE_SIZEで除算して、何本目の罫線と交点を持つかがわかる。floorで整数に直してから、タイルの大きさを乗じてグリッドとの交点を求める */
    if (this.isRayFacingDown){ /* rayが下方向に伸びてている時、プレイヤーの座標よりも下にある罫線との交点のy座標を取らなければならないので… TILE_SIZEを加算して、yinterseptを一つ下のグリッドの座標にする */
      yintercept += TILE_SIZE;
    }

    /* 同じく、最も近い横罫線との交点のx座標を取得する */
    xintercept = player.x + ((yintercept - player.y) / Math.tan(this.rayAngle));

    /* 次の交点を取得するために、x座標とy座標のそれぞれの増加分(xstep, ystep)を保持する */
    ystep = (this.isRayFacingDown) ? TILE_SIZE : -TILE_SIZE; /* rayが下方向のとき、TILE_SIZEを加算し、上方向のときはTILE_SIZEを減算する */

    xstep = TILE_SIZE / Math.tan(this.rayAngle);
    if (this.isRayFacingRight && xstep < 0){ /* rayが右向きの場合は、xstepは正の数でなければならない(繰り返し処理で右の座標へ動く必要がある)ので、符号を反転して正の数にする */
      xstep = -xstep;
    }else if (!(this.isRayFacingRight) && xstep > 0){ /* rayが左向きの場合は、xstepは負の数でなければならない(繰り返し処理で左の座標へ動く必要がある)ので、符号を反転して負の数にする */
      xstep = -xstep;
    }

    var nextHorzTouchX = xintercept;
    var nextHorzTouchY = yintercept;

    /* 壁と衝突するまで、交点を検査する */
    while (0 <= nextHorzTouchX && nextHorzTouchX <= WINDOW_WIDTH && 0 <= nextHorzTouchY && nextHorzTouchY <= WINDOW_HEIGHT){
      if (grid.hasWallAt(nextHorzTouchX, nextHorzTouchY - (!(this.isRayFacingDown) ? 1 : 0))){ /* 壁と衝突する交点が見つかったら、x, yの座標を保持してループを抜ける */
        foundHorzWallHit = true;
        horzWallHitX = nextHorzTouchX;
        horzWallHitY = nextHorzTouchY;
        break;
      }else{ /* 交点の座標で壁と衝突していない場合、次の交点の座標へ移る */
        nextHorzTouchX += xstep;
        nextHorzTouchY += ystep;
      }
    }

    /* 2. グリッドの縦罫線とrayの交点での、壁との距離を計算する */
    var foundVertWallHit = false;
    var vertWallHitX = 0;
    var vertWallHitY = 0;
        
    /* プレイヤーの座標と向きから、最も近い縦罫線との交点のx座標を取得する */
    xintercept = Math.floor(player.x / TILE_SIZE) * TILE_SIZE; /* TILE_SIZEで除算して、何本目の罫線と交点を持つかがわかる。floorで整数に直してから、タイルの大きさを乗じてグリッドとの交点を求める */
    if (this.isRayFacingRight){ /* rayが右方向に伸びてている時、プレイヤーの座標よりも右にある罫線との交点のx座標を取らなければならないので… TILE_SIZEを加算して、xinterseptを一つ右のグリッドの座標にする */
      xintercept += TILE_SIZE;
    }
    
    /* 同じく、最も近い縦罫線との交点のy座標を取得する */
    yintercept = player.y + ((xintercept - player.x) * Math.tan(this.rayAngle));
    
    /* 次の交点を取得するために、x座標とy座標のそれぞれの増加分(xstep, ystep)を保持する */
    xstep = (this.isRayFacingRight) ? TILE_SIZE : -TILE_SIZE; /* rayが右方向のとき、TILE_SIZEを加算し、左方向のときはTILE_SIZEを減算する */
    
    ystep = TILE_SIZE * Math.tan(this.rayAngle);
    if (this.isRayFacingDown && ystep < 0){ /* rayが下向きの場合は、ystepは正の数でなければならない(繰り返し処理で下の座標へ動く必要がある)ので、符号を反転して正の数にする */
      ystep = -ystep;
    }else if (!(this.isRayFacingDown) && ystep > 0){ /* rayが上向きの場合は、ystepは負の数でなければならない(繰り返し処理で上の座標へ動く必要がある)ので、符号を反転して負の数にする */
      ystep = -ystep;
    }
    
    var nextVertTouchX = xintercept;
    var nextVertTouchY = yintercept;
    
    /* 壁と衝突するまで、交点を検査する */
    while (0 <= nextVertTouchX && nextVertTouchX <= WINDOW_WIDTH && 0 <= nextVertTouchY && nextVertTouchY <= WINDOW_HEIGHT){
    if (grid.hasWallAt(nextVertTouchX - (!(this.isRayFacingRight) ? 1 : 0), nextVertTouchY)){ /* 壁と衝突する交点が見つかったら、x, yの座標を保持してループを抜ける */
        foundVertWallHit = true;
        vertWallHitX = nextVertTouchX;
        vertWallHitY = nextVertTouchY;
        break;
      }else{ /* 交点の座標で壁と衝突していない場合、次の交点の座標へ移る */
        nextVertTouchX += xstep;
        nextVertTouchY += ystep;
      }
    }

    /* 3. 横罫線で壁と衝突する距離と縦罫線で壁と衝突する距離を比較し、小さいほうの値をrayのメンバ変数に保持する */
  
    var horzHitDistance = (foundHorzWallHit)
        ? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY) /* 壁と衝突していたら、その距離を保持 */
        : Number.MAX_VALUE; /* 衝突する座標がない場合、数値の最大値を入れておく(比較時に混乱しないように) */
    var vertHitDistance = (foundVertWallHit)
        ? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
        : Number.MAX_VALUE;

    this.wallHitX = (horzHitDistance < vertHitDistance) ? horzWallHitX : vertWallHitX;
    this.wallHitY = (horzHitDistance < vertHitDistance) ? horzWallHitY : vertWallHitY;
    this.distance = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;
    this.wasHitVertical = (horzHitDistance < vertHitDistance); /* 縦罫線と横罫線のどちらで壁に衝突したかを保持する */
  }
  render(){
    stroke("rgba(255, 0, 0, 0.3)");
    line( /* playerの現在位置から、壁と衝突するところまで直線を描画する */
      player.x * MINIMAP_SCALE_FACTOR,
      player.y * MINIMAP_SCALE_FACTOR,
      this.wallHitX * MINIMAP_SCALE_FACTOR,
      this.wallHitY * MINIMAP_SCALE_FACTOR
      );
    // line( /* playerの現在位置から、rayAngle の方向に30px動かした座標まで直線を描画する */
    //   player.x,
    //   player.y,
    //   player.x + Math.cos(this.rayAngle) * 50, 
    //   player.y + Math.sin(this.rayAngle) * 50  
    // );
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
  var rayAngle = player.rotationAngle - (FOV_ANGLE / 2); /* 最初のrayの角度は、プレイヤーの向きからFOVの半分を引いた値(プレイヤーの向きは、FOVの中心なので) */
  rays = [];

  for (var i=0; i<NUM_RAYS; i++){
    var ray = new Ray(rayAngle);
    ray.cast();
    rays.push(ray);
    rayAngle += FOV_ANGLE / NUM_RAYS; /* 次のrayの角度は、rayの数でFOVを除算した値だけ変化する */
  }
}

/*
 ** rayごとの計算結果をもとに、壁を描画する
 */
function render3DProjectedWalls(){
  for(var col = 0; col < NUM_RAYS; col++)
  {
    var ray = rays[col];
    
    /*
     ** プレイヤーの視野範囲(FOV_ANGLE)が画面内にすべて収まるためには、画面との距離を計る必要がある
     ** 三角関数を利用して、プレイヤーの視点と、画面平面との垂直な交点を算出する
     */
    var distanceProjectionPlane = (WINDOW_WIDTH / 2) / Math.tan(FOV_ANGLE / 2);

    /*
     ** rayと壁の距離で描画すると、画面の端を描画するrayほど、距離が長く計算されてしまう
     ** 距離が長くならないように、視野の中央で交差する場合の距離に直しておく必要がある
     */
    var correctWallDistance = ray.distance * Math.cos(ray.rayAngle - player.rotationAngle);

    /*
     ** 三角形の相似形の性質により、描画すべき壁の高さは以下で算出される
     ** (壁の実寸(TILE_SIZE) / (プレイヤーから壁までの距離(ray.distance) = (画面に描画される壁の大きさ) / (プレイヤーから画面までの距離)))
     ** この式を変形して、画面上の壁の高さの値がわかる
     */
    var wallStripHeight = (TILE_SIZE / correctWallDistance) * distanceProjectionPlane;

    /*
     ** 縦罫線で壁に衝突した場合と、横罫線で壁に衝突した場合で壁の色を分ける(光の当たり方の表現)
     ** rayのメンバで保持しているwasHitVert で、どちらの罫線に当たったかで条件分岐する
     */
    var wallCollor = 255;
    if (ray.wasHitVertical)
    {
      wallCollor = 188;
    }

    /*
     ** 壁との距離が遠いほど暗く表示する
     ** 表示する色のalphaを少なくすることで表現する
     */

    var alpha = 200 / correctWallDistance;

    fill(
      "rgba("
        + wallCollor + ", "
        + wallCollor + ", "
        + wallCollor + ", "
        + alpha
      + ")"
    );
    noStroke();
    rect(
      col * WALL_STRIP_WIDTH, /* 左からn列目の位置 */
      (WINDOW_HEIGHT / 2) - (wallStripHeight / 2), /* 壁の中心が、画面の中心にくる位置 */
      WALL_STRIP_WIDTH, /* 描画する壁の幅 */
      wallStripHeight /* 描画する壁の高さ */
    );
  }

}

/*
 ** normalizeAngle(angle):
 ** 与えられた角度を 0 ~ 2π の範囲に変換する
 */
function normalizeAngle(angle){
  angle = angle % (2 * Math.PI); /* 剰余算で2πを超える部分を切り捨てする */
  if (angle < 0){ /* 負の値のときは、2πから減算した値に変換する */
    angle = (2 * Math.PI) + angle;
  }
  return (angle);
}

function distanceBetweenPoints(x1, y1, x2, y2){
  /* 三平方の定理で、2点間の距離を導く */
  /* 直角三角形の各辺において、以下が成り立つ */
  /* (斜辺)^2 = (隣辺)^2 + (対辺)^2 */
  return (Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 -y1)));
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
  clear();
  update();

  render3DProjectedWalls();

  grid.render();
  for (ray of rays){ /* 保持したrayを1本ずつ描画する */
    ray.render();
  }
  player.render();
}