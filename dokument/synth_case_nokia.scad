$fn = 20;

include <roundCornersCube.scad>



/////CASE SIZE/////
//case length (outer)
cl = 175;

//case width (outer)
cw = 90;

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
ckhw = 45;

//case key holes length
ckhl = 110;

//case display hole length
cdhl = 37;

//case display hole width
cdhw = 32;

//case display hole radius
cdhr = 2;




module button_holders()
{
holder(r=1.20,h=10-1.6);

translate([91.44,0,0])
holder(r=1.20,h=10-1.6);


translate([91.44,15.875,0])
holder(h=10-1.6);


translate([0,15.875,0])
holder(h=10-1.6);



translate([44.45,0,0])
holder(r=1.2,h=10-1.6);


translate([44.45,7.62,0])
holder(h=10-1.6);


}


module pcb_holders()
{
holder(h=3);

translate([78.74,-28.575,0])
holder(h=3);

translate([0,-28.575,0])
holder(h=3);


translate([78.74,0,0])
holder(h=3);


}

module corner_holders()
{
translate([6,6,-cbh+cbth])
holder(h=10);


translate([6,85-1,-cbh+cbth])
holder(h=10);


translate([170-1,5+1,-cbh+cbth])
holder(h=10);


translate([170-1,85-1,-cbh+cbth])
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


translate([170-1,85-1,-cbh+cbth])
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
  rCube(cl, cw, cbh,crr);

  translate([cwth,cwth,ctth])
  cube([cl-cwth*2, cw-cwth*2, cbh]);

  }
  
}

module key_holder()
{
  translate([0,0,2])
  difference()
  {
  cube([106,5,3]);


  for(i=[0:6])
  {
  translate([8+i*106/7,2.5,-2])
  cylinder(r=1.4, h=12);
  }
  }
}

module case_top()
{

  //spacers
  //translate([cwth*2+40,43,10])
  //holder(h=5, w=2, r=.9);

  translate([cwth*2+2,46,10])
  holder(h=5, w=2, r=.9);


  translate([cwth*2+2,46+40,10])
  holder(h=5, w=2, r=.9);

  translate([cwth*2+40+2,46+40,10])
  holder(h=5, w=2, r=.9);


  //spacers for 4 buttons
  translate([90,55,10])
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
    translate([cwth*3,52,0])
    rCube(cdhl, cdhw, 22, cdhr);

    //speaker hole
    translate([cl-25-cwth,cwth*1+25,8])
    color("red")
    //cylinder(r=25, h=10);
    speaker_hole();


    //pot hole
    translate([100, 68, 10])
    cylinder(r=4, h=10);

    //4 buttons holes
  translate([90-8.255,55,10])
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


corner_holders();
}


//translate([80,-2,-30])
//cube([.5,200,200]);

}
*/

rotate([0,180,0])
difference()
{
union()
{
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
