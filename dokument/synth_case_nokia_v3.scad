$fn = 20;

include <roundCornersCube.scad>
include <all_keys.scad>


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
cth = 15+ctth-3;

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
cdhw = 32-4;

//case display hole radius
cdhr = 2;


module button_holders()
{
holder(r=1.20,h=9-1.6);

translate([91.44,0,0])
holder(r=1.20,h=9-1.6);


translate([91.44,15.875,0])
holder(h=9-1.6);


translate([0,15.875,0])
holder(h=9-1.6);



//translate([44.45,0,0])
translate([46.99,0,0])
holder(r=1.2,h=9-1.6);


//translate([44.45,7.62,0])
translate([46.99,7.62,0])
holder(h=9-1.6);

}

module pcb_holders()
{
holder(h=4, r=1.5); //changed since v2!

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
holder(h=10, w=2.5);


//translate([6,85-1+5,-cbh+cbth])
//holder(h=10);


translate([170-1,5+1,-cbh+cbth])
holder(h=10);


translate([170-1,85-1+5,-cbh+cbth])
holder(h=10);


}


module 4_button()
{
cylinder(d=13.5, h=.75);
translate([0,0,.75])
cylinder(d=8.7, h=1.2+2);


}

module 4_buttons()
{
holder(h=5);
translate([0,36.195,0])
holder(h=5);

translate([26.67,36.195,0])
holder(h=5);

translate([26.67,0,0])
holder(h=5);
}

module button_hole()
{
cylinder(r=5,h=10);
translate([0,0,2])
cylinder(r=5+2.2,h=1.25);
}

module 4_buttons_holes()
{

translate([13.335, 3.175,-.1])
{
button_hole();
}



translate([3.175, 13.97,-.1])
{
//cylinder(r=5,h=10);
button_hole();
}

translate([13.335, 24.765,-.1])
//cylinder(r=5,h=10);
button_hole();


translate([23.495, 13.97,-.1])
//cylinder(r=5,h=10);
button_hole();
}


module corner_holders_top()
{
translate([6,6,-cth+2*ctth])
holder(h=cth-ctth,w=2);


//translate([6,85-1,-cbh+cbth])
//holder(h=10, w=3);


translate([170-1,5+1,-cth+2*ctth])
holder(h=cth-ctth, w=3);


translate([170-1,85-1+5,-cth+2*ctth])
holder(h=cth-ctth, w=3);


//in lcd housing
translate([cwth+48/2,101,-cth+2*ctth])
holder(h=cth-ctth, w=1.8);

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
  translate([155-2.54*5,5,8.5])
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
  translate([0,0,-1])
  difference()
  {
  cube([106,8,3]);

  for(i=[0:4])
  {
  translate([wktw/2+i*(dbwk*7-key_tol-wktw)/4,2.5+1,-3])
  cylinder(r=1.4, h=120);
  }
  }


}

module holder_at_key(width=wktw)
{
  translate([wktw-width,0,0])
  {
  cube([width,5+1,1.4]);

  translate([0,3.4+1,0])
  cube([width,1.6,3.5]);

  }
  
}


module holder_at_key_v2(width=wktw-bktw/2-key_tol/2)
{
  translate([wktw-width,0,0])
  {


  difference()
  {
  cube([width,8,1.4]);
  translate([width/2,4,-2])
  cylinder(r=1.6, h=10);
  }
  }
  
}

module button_cap()
{
  //button diameter
  bd = 8.5;

  //button height
  bh = 2;

  //button flange diameter
  bfd = 11;

  //button flange height
  bfh = 1;

  //button inset diameter
  bid = 4.5;

  //button inset depth
  bide = 0;


  difference()
  {
    union()
    {
      cylinder(d=bfd, h=bfh);
      translate([0,0,bfh])
      cylinder(d=bd, h=bh);
    }
    
    cylinder(d=bid, h=bide);
  }
  
}


module case_top()
{

  //LCD spacers
  translate([cwth*2+40-2,46+10,cth-ctth-5])
  holder(h=5, w=2, r=.95);

  translate([cwth*2,46+10,cth-ctth-5])
  holder(h=5, w=2, r=.95);

  translate([cwth*2,46+40+10,cth-ctth-5])
  holder(h=5, w=2, r=.95);

  translate([cwth*2+40,46+40+10,cth-ctth-5])
  holder(h=5, w=2, r=.95);


  //spacers for 4 buttons
  translate([90,55+2,cth-ctth-5])
  rotate([0,0,90])
  4_buttons();


  //holders for keys
  translate([cwth*2+4+2,44,10]) //+2 compared to v2!
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
    translate([cl-23-cwth,cwth*1+22,8])
    color("red")
    //cylinder(r=25, h=10);
    speaker_hole();


    //pot hole
    translate([102, 68+2, 10])
    cylinder(r=4, h=10);


    //ISP programmer hole
    translate([140-25, 68-5, 10])
    rCube(30,20,10,8);



    //lcd inset
    translate([3,76,-3])
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
  for(i=[0:4])
  {
    translate([i*7,0,0])
    rCube(1.5,30,10,1);
  }

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


module battery_holder(width=54.5)
{


cube([3,52,5]);
translate([width+3,0,0])

rotate([0,-3,0])
translate([.5,52-20,0])
cube([2.5,20,12]);


/*
translate([width+3,52-20,14])
rotate([-90,0,0])
difference()
{
//cylinder(r=2,h=20, $fn=4);
//translate([-2,-20,-.2])
//cube([20,20,21]);
}
*/


//wedge type thing at the back
difference()
{
translate([width+3+3,52-20,0])
rotate([-90,0,0])
cylinder(r=2.5,h=20, $fn=4);

translate([width,52-21,-5.01])
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

translate([18+3,16,-cbh+cbth]) //+3 in difference between v3 and v2
button_holders();


translate([88,76,-cbh+cbth])
pcb_holders();

//corner_holders();
}


color("red")
translate([23,37,-cbh+cbth-1.25])
rCube(55,52.5,1.3,1.2);


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

//translate([80+10,-2,-30])
//cube([.5,100,100]);


}



//holes for key holder
translate([cwth*2+4+2,44,10]) //+2 compared to v2!
for(i=[0:4])
  {
  translate([wktw/2+i*(dbwk*7-key_tol-wktw)/4,2.5+1,-.5])
  cylinder(r=1.4, h=4);
  }
}
*/


4_button();

//speaker
//translate([140,0,0])
//cylinder(d=50, h=25);

//translate([-120+2+1,7,-10])
//all_white_keys();
