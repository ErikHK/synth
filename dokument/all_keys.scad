$fn = 20;
//white key total length
wktl = 42;

//black key total length
bktl = 28;

//white key total width
wktw = 12.8;

//black key total width
bktw = 5.5;

//distance between keys
dbk = 8;


//white key thickness
wkth = 2;

//black key thickness
bkth = 4;

//white key lip height
wklh = 5;

//spring length
sl = 3;

//spring width
sw = 10;

//spring thickness
sth = .5;

//key tolerance
key_tol = .8;

//distance between white keys
dbwk = wktw+key_tol;


module white_key(leftnotch=true, rightnotch=true)
{
  //slim width
  sw = wktw-bktw-key_tol;

  

  difference()
  {
  union()
  {
  cube([wktw,wktl,wkth]);
  translate([0,-wkth,0])
  cube([wktw,wkth,wklh]);
  }

  //notch for black key
  if(leftnotch)
  {
  translate([-.01,wktl-bktl-key_tol/2+.01,-.1])
  cube([bktw/2+key_tol/2,bktl+key_tol/2,5]);

  }

  if(rightnotch)
  {
  translate([.01+wktw-bktw/2-key_tol/2,wktl-bktl-key_tol/2+.01,-.1])
  cube([bktw/2+key_tol/2,bktl+key_tol/2,5]);
  

  //translate([-.01,wktl-bktl-key_tol/2+.01,-.1])
  //cube([bktw/2+key_tol/2,bktl+key_tol/2,5]);



  }


  }
  
  /*
  translate([0,wktl+sl,0])
  color("brown")
  translate([bktw/2+key_tol/2,0,0])
  {


  difference()
  {
  cube([sw,8,wkth]);
  translate([sw/2,4,-2])
  cylinder(r=1.6, h=10);
  }
  }
  */

  //spring
  translate([wktw/2-sw/2,wktl,wkth-sth])
  cube([sw,sl,sth]);


  //translate([wktw/2,wktl-bktl,wkth])
  //foil_holder();

}


module black_key()
{
  cube([bktw, bktl, bkth]);

  //spring
  translate([0,bktl,wkth-sth])
  cube([bktw,sl,sth]);


  //translate([bktw/2,0,bkth])
  //foil_holder();

}

module foil_holder()
{
  height=2;
  diameter=6;
  thickness=1;

  translate([0,diameter/2,0])
  difference()
  {
    cylinder(d=diameter, h=height);
    cylinder(d=diameter-thickness*2, h=height);
  }


}


module holder_bar(length=dbwk*7-key_tol)
{
  width = 8;
  ridge_width=3;
  ridge_height=1;
  tol = .8;

  square_width=3;

  difference()
  {
  union()
  {
  cube([length,width,wkth]);
  //ridge
  translate([0,width/2-ridge_width/2,0])
  cube([length,ridge_width,wkth+ridge_height]);

  }

  for(i=[0:4])
  translate([wktw/2+i*(length-wktw)/4,width/2,-1])
  cylinder(d=2.5,h=10);


  //cubes
  for(i=[0:4])
  translate([wktw/2+i*(length-wktw)/4,width/2,wkth+ridge_height/2])
  //cylinder(d=2.5,h=10);
  cube([square_width+tol,ridge_width+tol,ridge_height], center=true);



  //ridge
  translate([0,width/2-ridge_width/2-tol/2,-2])
  cube([length,ridge_width+tol,wkth+ridge_height]);
  }


  //cubes with cylindrical holes
  for(i=[0:4])
  {
  translate([wktw/2+i*(length-wktw)/4,width/2,ridge_height/2])
  difference()
  {
  cube([square_width,ridge_width+tol,ridge_height], center=true);
  translate([0,0,-5])
  cylinder(d=2.5,h=10);
  }

  }


}

module all_black_keys()
{
  translate([-dbwk+bktw/2+key_tol/2,bktl+sl,0])
  holder_bar();


  black_key();
  translate([1*dbwk, 0,0])
  black_key();

  translate([2*dbwk, 0,0])
  black_key();

  translate([4*dbwk, 0,0])
  black_key();

  translate([5*dbwk, 0,0])
  black_key();
}


module all_white_keys()
{
  //total length
  tl = dbwk*7-key_tol;

  translate([0,wktl+sl,0])
  holder_bar();


  translate([wktw/2-2,15,2])
  foil_holder();


  translate([wktw/2-2+2*8,15,2])
  foil_holder();


  translate([wktw/2-2+4*8-2.5,15,2])
  foil_holder();


  translate([wktw/2-2+6*8-2.5,15,2])
  foil_holder();

  translate([wktw/2-2+7*8-1,15,2])
  foil_holder();

  translate([wktw/2-2+9*8-1.5,15,2])
  foil_holder();


  translate([wktw/2-2+11*8-1.5,15,2])
  foil_holder();


white_key(false,true);
translate([dbwk, 0,0])
white_key();

translate([2*dbwk, 0,0])
white_key();

translate([3*dbwk, 0,0])
white_key(true,false);


translate([4*dbwk, 0,0])
white_key(false,true);


translate([5*dbwk, 0,0])
white_key(true,true);


translate([6*dbwk, 0,0])
white_key(true,false);
}

//translate([dbwk-bktw/2-key_tol/2,wktl-bktl,-3])
//all_black_keys();
all_white_keys();