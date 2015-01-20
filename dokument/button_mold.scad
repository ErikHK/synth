$fn = 80;

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


for(i=[0:12])
{
  translate([x[i],0,0])
  dome_hole();

}