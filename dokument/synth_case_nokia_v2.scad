$fn = 20;

include <roundCornersCube.scad>



/////CASE SIZE/////
//case length (outer)
cl = 175;

//case width (outer)
cw = 95;

//case bottom thickness
cbth = 2;


//case bottom height
cbh = 10+cbth;

//case top thickness
ctth = 2;


//case top height
cth = 15+ctth;

//case roundness radius
crr = 2.5;

//case wall thickness
cwth = 3;

//case key holes width
ckhw = 43;

//case key holes length
ckhl = 110;

//case display hole length
cdhl = 37;

//case display hole width
cdhw = 32;

//case display hole radius
cdhr = 2;


/////KEY SIZES/////
//white key total length
wktl = 42;

//black key total length
bktl = 20;

//white key total width
wktw = 12.5;

//black key total width
bktw = 7;

//distance between keys
dbk = 8;

//white key thickness
wkth = 2;

//white key lip height
wklh = 5;

//key tolerance
key_tol = 1;

module button_holders()
{
holder(r=1.20,h=10-1.6);

translate([91.44,0,0])
holder(r=1.20,h=10-1.6);


translate([91.44,15.875,0])
holder(h=10-1.6);


translate([0,15.875,0])
holder(h=10-1.6);



//translate([44.45,0,0])
translate([46.99,0,0])
holder(r=1.2,h=10-1.6);


//translate([44.45,7.62,0])
translate([46.99,7.62,0])
holder(h=10-1.6);


}


module pcb_holders()
{
holder(h=4, r=0.95);

translate([78.74,-28.575,0])
holder(h=4, r=0.95);

translate([0,-28.575,0])
holder(h=4, r=.95);


translate([78.74,0,0])
holder(h=4, r=.95);


}

module corner_holders()
{
translate([6,6,-cbh+cbth])
holder(h=10);


//translate([6,85-1+5,-cbh+cbth])
//holder(h=10);


translate([170-1,5+1,-cbh+cbth])
holder(h=10);


translate([170-1,85-1+5,-cbh+cbth])
holder(h=10);


}


module 4_button()
{
cylinder(r=6, h=2);
translate([0,0,2])
cylinder(r=4.5, h=4);


}

module 4_buttons()
{
holder();
translate([0,36.195,0])
holder();

translate([26.67,36.195,0])
holder();

translate([26.67,0,0])
holder();
}

module 4_buttons_holes()
{
translate([13.335, 3.175,0])
cylinder(r=5,h=10);

translate([3.175, 13.97,0])
cylinder(r=5,h=10);


translate([13.335, 24.765,0])
cylinder(r=5,h=10);


translate([23.495, 13.97,0])
cylinder(r=5,h=10);
}


module corner_holders_top()
{
translate([6,6,-cbh+cbth])
holder(h=15,w=2);


//translate([6,85-1,-cbh+cbth])
//holder(h=10, w=3);


translate([170-1,5+1,-cbh+cbth])
holder(h=15, w=3);


translate([170-1,85-1+5,-cbh+cbth])
holder(h=15, w=3);


}

module holder(r=1.5, h=5, w=4)
{
difference()
{
cylinder(r=r+w, h=h);
cylinder(r=r, h=h);
}
}

module atmega()
{
color("yellow")
//cube([53, 16, 5]);
cube([85, 35, 1.6]);
}


module AAA_battery()
{
  cylinder(r=10.2/2, h=44.25);
  translate([0,0,44.25])
  cylinder(r=3.3/2, h=1);
}



module small_display()
{
//display outer
do = [58, 32, 1.6];
//display display
dd = [71.2, 25.2, 13.5];

cube(do);

}


module display()
{
//display outer
do = [80, 36, 1.6];
//display display
dd = [71.2, 25.2, 13.5];

cube(do);
translate([4.4,5.4,-3])
color("lightblue")
cube(dd);

module hole()
{
difference()
{
color("red")
cylinder(r=2.5, h=1.61);
cylinder(r=1, h=18);
}
}
//holes
translate([2.5, 2.5,0])
hole();

translate([2.5, do[1]-2.5,0])
hole();

translate([do[0]-2.5, do[1]-2.5,0])
hole();

translate([do[0]-2.5,2.5,0])
hole();
}


module white_key()
{


}


module case_bottom()
{
  

  difference()
  {
  union()
  {
  translate([0,60,0])
  lcd_housing_bottom();

  //holder in lcd housing
  translate([3+48/2,100.5,2])
  holder(h=10);


  //headphone jack holders
  translate([155,6,2])
  holder(r=1.4,h=3);

  translate([155-2.54*10,6,2])
  holder(r=1.4,h=3);

  difference()
  {
  rCube(cl, cw, cbh,crr);
  

  translate([cwth,cwth,ctth])
  cube([cl-cwth*2, cw-cwth*2, cbh]);

  //lcd housing hole
  translate([3,90,2])
  cube([48,6,12]);

  //headphone jack hole
  translate([155-2.54*5,5,10])
  rotate([90,0,0])
  cylinder(d=6, h=10);
  }

  translate([0,0,cbh])
  corner_holders();
  }

  //sunk holes
  translate([6,6,-.01])
  cylinder(r=3.5, h=5);

  translate([170-1,5+1,-.01])
  cylinder(r=3.5, h=5);

  translate([170-1,85-1+5,-.01])
  cylinder(r=3.5, h=5);
  
  //lcd housing sunk hole
  translate([3+48/2,100.5,-.01])
  cylinder(r=3.5, h=5);
  }
}

module key_holder()
{
  translate([0,-2,0])
  difference()
  {
  translate([0,0,-3])
  cube([106,8,8]);

  translate([0,0,3])
  cube([108,5,2]);

  translate([0,3,0])
  cube([108,2,4]);  
  }

  //support
  color("red")
  translate([0,-2,2])
  cube([106,.25,3]);


  color("red")
  translate([0,-.25+1,2])
  cube([106,.25,3]);


}

module holder_at_key(width=wktw)
{
  translate([wktw-width,0,0])
  {
  cube([width,5,1.4]);

  translate([0,3.4,0])
  cube([width,1.6,3.5]);

  }
  
}

module foil_holder(width=wktw)
{
  translate([wktw-width,0,0])
  difference()
  {
  cube([width,8,3]);
  translate([1,1,0])
  cube([width-2,6,6]);
  }
}


module white_key_1(notch=true)
{
  difference()
  {
  union()
  {
  cube([wktw,wktl,wkth]);
  translate([0,-wkth,0])
  cube([wktw,wkth,wklh]);
  }

  //notch for black key
  if(notch)
  translate([-.01,wktl-bktl-key_tol/2+.01,-.1])
  cube([bktw/2+key_tol/2,bktl+key_tol/2,5]);

  }
  
  color("red")
  translate([0,38-23-2,2])
  if(!notch)
    foil_holder();
  else
    foil_holder(width=wktw-bktw);

  translate([0,wktl,2])
  color("brown")
  if(!notch)
    holder_at_key();
  else
    holder_at_key(width=wktw-bktw-key_tol/2);

  if(notch)
    translate([bktw+key_tol/2,wktl-2,2])
    cube([wktw-bktw-key_tol/2,2,1.4]);
  else
    translate([0,wktl-2,2])
    cube([wktw,2,1.4]);
}

rotate([0,-90,0])
white_key_2();

translate([20,0,0])
rotate([0,90,0])
white_key_1();


translate([40,0,0])
rotate([0,90,0])
white_key_3();


module white_key_2()
{
mirror([1,0,0])
white_key_1();

}

module white_key_3()
{
white_key_1(false);
}

module case_top()
{

  //LCD spacers
  translate([cwth*2+40-2,46+10,10])
  holder(h=5, w=2, r=.95);

  translate([cwth*2,46+10,10])
  holder(h=5, w=2, r=.95);

  translate([cwth*2,46+40+10,10])
  holder(h=5, w=2, r=.95);

  translate([cwth*2+40,46+40+10,10])
  holder(h=5, w=2, r=.95);


  //spacers for 4 buttons
  translate([90,55+2,10])
  rotate([0,0,90])
  4_buttons();


  //holders for keys
  translate([cwth*2+4,45,10])
  key_holder();


  difference()
  {
    rCube(cl, cw, cth, crr);
    translate([cwth*3,-.1,-1])
    {
      rCube(ckhl, ckhw, cth*2, 4);
      translate([0,-4,0])
      cube([ckhl, ckhw, cth*2]);
    }

    translate([cwth,cwth,-.01])
    cube([cl-cwth*2, cw-cwth*2, cth-ctth]);

    //display hole
    translate([cwth*3-2,52+10-2,0])
    rCube(cdhl, cdhw, 22, cdhr);

    //speaker hole
    translate([cl-25-cwth,cwth*1+25,8])
    color("red")
    //cylinder(r=25, h=10);
    speaker_hole();


    //pot hole
    translate([160, 68, 10])
    cylinder(r=4, h=10);


    //ISP programmer hole
    translate([140-25, 68-5, 10])
    rCube(30,20,10,8);



    //lcd inset
    translate([3,76,0])
    cube([48,20,15]);


    //4 buttons holes
  translate([90-8.255,55+2,10])
  rotate([0,0,90])
  4_buttons_holes();


  }


  

}

module batteries()
{
translate([50,90,-6])
{
rotate([-90,0,90])
//translate([190,0,0])
{
AAA_battery();

translate([0,0,-46])
AAA_battery();


translate([0,0,-46*2])
AAA_battery();


translate([0,0,-46*3])
AAA_battery();


}
}
}

module speaker_hole(d=50)
{
//cylinder(d=d, h=5);

rotate([0,0,-45])
translate([-3-15,-15,-1])
{
rCube(6,30,10,3);

translate([15,0,0])
rCube(6,30,10,3);

translate([15*2,0,0])
rCube(6,30,10,3);

}

}



module lcd_housing()
{
  difference()
  {
    rCube(48+cwth*2,40+cwth*2,cth,8);
    translate([cwth, cwth, -cbth])
    rCube(48, 40,cth, 8);
  translate([0,-2,-2])
  cube([55,34,40]);

  }
}


module lcd_housing_bottom()
{
  difference()
  {
    rCube(48+cwth*2,40+cwth*2,cbh,8);
    translate([cwth, cwth, cbth])
    rCube(48, 40,cbh, 8);
  translate([0,-2,-2])
  cube([55,34,40]);

  }
}


module battery_holder(width=56)
{
cube([3,52,5]);
translate([width+3,0,0])

translate([0,52-20,0])
cube([3,20,14+4]);

translate([width+3,52-20,16])
rotate([-90,0,0])
cylinder(r=2,h=20, $fn=4);

difference()
{
translate([width+3+3,52-20,0])
rotate([-90,0,0])
cylinder(r=3,h=20, $fn=4);

translate([width,52-21,-5])
cube([20,25,5]);
}


}


//lcd_housing();

//speaker_hole();

//translate([cwth*2,50,-2.4+2])
//display();

//translate([0,0,20])
//small_display();
//case_top();

//translate([96,55,10])
//atmega();

//batteries();
/*
translate([20,85,0])
rotate([90,0,0])
AAA_battery();

translate([32,85,0])
rotate([90,0,0])
AAA_battery();


translate([44,85,0])
rotate([90,0,0])
AAA_battery();


translate([56,85,0])
rotate([90,0,0])
AAA_battery();
*/

/*
translate([20,35,-10])
battery_holder();

difference()
{
union()
{
translate([0,0,-cbh])
case_bottom();

translate([18,16,-cbh+cbth])
button_holders();


translate([88,76,-cbh+cbth])
pcb_holders();

//corner_holders();
}


//translate([80,-2,-30])
//cube([.5,200,200]);

}
*/
/*
rotate([0,180,0])
difference()
{
union()
{
translate([0,60,0])
lcd_housing();
translate([0,0,10])
corner_holders_top();
case_top();
}

//translate([80+10,-2,-30])
//cube([.5,100,100]);


}

//4_button();

//speaker
//translate([140,0,0])
//cylinder(d=50, h=25);
*/
