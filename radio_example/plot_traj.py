import h5py
from mpl_toolkits.mplot3d import axes3d
import radiopropa 

from Ice import iceModel


#Plot Ice model
z = linspace(-300,0)

n = zeros_like(z)
dn = zeros_like(z)

position = radiopropa.Vector3d(0,0,0) 

for i,vz in enumerate(z):
    position.z = vz
    n[i] = iceModel.getValue(position)
    dn[i] = iceModel.getGradient(position).z


figure()
plot(n, z)
xlabel('Index of Refracton')
ylabel('Z [m]')




# plot simulated data
f = h5py.File('output_traj.h5')
d = f['Trajectory3D']

figure()
s1 = subplot(111)

SN = set(d['SN'])
for s in SN:
    idx = d['SN'] == s
    X = d['X'][idx]
    Z = d['Z'][idx]
    dX = X[1] - X[0]
    dZ = Z[1] - Z[0]
    phi0 = abs(arctan(dX/dZ))

    c = cm.jet(phi0 / pi * 2)
    print phi0

    s1.plot(d['X'][idx], d['Z'][idx], c=c, label='RadioPropa')


s1.set_xlabel('X [m]')
s1.set_ylabel('Z [m]')
tight_layout()




#xlim(0,10)
#ylim(-240, -230)
