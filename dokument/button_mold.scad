$fn = 20;


include <roundCornersCube.scad>
x = [7.62, 7.62, 15.24, 22.86,31.75, 40.64, 49.53, 58.42, 67.31, 76.2, 83.82,91.44];

module dome(d=5, h=.7)
{
  c = d;
  R = h/2+c*c/(8*h);
  difference()
  {
  translate([0,0,-R+h])
  sphere(r=R);
  translate([-R*2,-R*2,-R*2])
  cube([R*4,R*4,R*2]);
  }
  
}

module dome_hole()
{
difference()
{
dome(h=.7+1, d=5+2);

dome(h=.7, d=5);

}

}

module dome_bool()
{

//dome(h=.6, d=5);
cylinder(d1=5, d2=3, h=1);
}


module domes()
{
for(i=[0:12])
{
  translate([x[i],0,0])
  dome_hole();

}
}

module dome_bools()
{
for(i=[0:12])
{
  translate([x[i],0,0])
  dome_bool();

}

}

module wholeshit()
{
union()
{
translate([0,0,0])
difference()
{
union()
{
translate([-4, -4, 0])
rCube(10*10,8,2,1);

//sprues
translate([-3, -3, -3])
cylinder(d=2, h=3);


translate([-3, 3, -3])

cylinder(d=2, h=3);

translate([-3+30, -3, -3])
cylinder(d=2, h=3);


translate([-3+30, 3, -3])
cylinder(d=2, h=3);


translate([-3+60, -3, -3])
cylinder(d=2, h=3);


translate([-3+60, 3, -3])
cylinder(d=2, h=3);


translate([-3+97, -3, -3])
cylinder(d=2, h=3);


translate([-3+97, 3, -3])
cylinder(d=2, h=3);


//domes();
}
//translate([0,0,50])
translate([0,0,0])
dome_bools();
}
}
}

difference()
{
translate([-8,-7.5,-3.2])
cube([110, 15, 5.19]);

wholeshit();

//screw holes
translate([0,0,-4])
{
translate([-6,5.5,0])
cylinder(d=2, h=30);

translate([-6,-5.5,0])
cylinder(d=2, h=30);


translate([-6+50,5.5,0])
cylinder(d=2, h=30);

translate([-6+50,-5.5,0])
cylinder(d=2, h=30);

translate([-6+105,5.5,0])
cylinder(d=2, h=30);

translate([-6+105,-5.5,0])
cylinder(d=2, h=30);
}
}



translate([0,30,0])
{
difference()
{
translate([-8,-7.5,-3.2])
cube([110, 15, 2]);

translate([0,0,-4])
{
translate([-6,5.5,0])
cylinder(d=2, h=30);

translate([-6,-5.5,0])
cylinder(d=2, h=30);


translate([-6+50,5.5,0])
cylinder(d=2, h=30);

translate([-6+50,-5.5,0])
cylinder(d=2, h=30);

translate([-6+105,5.5,0])
cylinder(d=2, h=30);

translate([-6+105,-5.5,0])
cylinder(d=2, h=30);
}
}
}