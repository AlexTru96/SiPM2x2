program lectura

IMPLICIT NONE
INTEGER :: count,k,xl1,id,ierr
real :: px,py,pz,x,y,z,mass,energy,pt,dirx,diry,dirz

mass= 0.000510998 !Gev/c2
!px, py, pz (Gev/c)
count=0
OPEN(UNIT=1,FILE=" ****** ",FORM="FORMATTED",STATUS="OLD",ACTION="READ") ! ***** = your file
open(unit=40,file='vertical_muon.dat',STATUS="UNKNOWN",ACTION="WRITE")

READ(1,'(////)') 

do 
   READ(1,156,IOSTAT=ierr) id,px,py,pz,x,y,z
        IF(ierr.LT.0) then
		EXIT 
	endif
	if(id==5 .OR. id == 6) then
                pt= (px**2+py**2+pz**2)
		energy= sqrt(pt+mass**2)
   		write(40,155) id,count,40*(2*rand()-1),40*(2*rand()-1),80,0.0,0.0,-1.0,energy
   		count= count + 1
	endif
end do

   print *,"servido"

close(40)
close(1)
155 FORMAT(I0,1X,I0,2(ES13.5),1X,I3,1X,3(F5.2,1X),ES12.5)
156 FORMAT(I4,1X,6(ES13.5))
end program
