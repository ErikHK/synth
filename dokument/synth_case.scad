$fn = 20;

include <roundCornersCube.scad>


/////CASE SIZE/////
//case length (outer)
cl = 190;

//case width (outer)
cw = 105;

//case bottom height
cbh = 10;

//case bottom thickness
cbth = 2;

//case top height
cth = 10;


//case top thickness
ctth = 2;

//case roundness radius
crr = 2.5;

//case wall thickness
cwth = 3;

//case key holes width
ckhw = 45;

//case key holes length
ckhl = 110;

//case display hole length
cdhl = 73;

//case display hole width
cdhw = 26;

//case display hole radius
cdhr = 2;



module atmega()
{
color("yellow")
//cube([53, 16, 5]);
cube([85, 30, 1.6]);
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

module case_top()
{

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
    translate([cwth*3,55,0])
    rCube(cdhl, cdhw, 22, cdhr);

  }


  //speaker hole
  translate([cl-25-cwth,cwth*1+25,4])
  color("red")
  cylinder(r=25, h=10);

}

translate([cwth*2,50,-2.4+2])
display();

//translate([0,0,20])
//small_display();
case_top();
translate([0,0,-cbh])
case_bottom();
translate([96,55,10])
atmega();
translate([0,94,15])

rotate([-90,0,90])
//translate([190,0,0])
AAA_battery();