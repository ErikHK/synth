$fn = 80;

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

dome();