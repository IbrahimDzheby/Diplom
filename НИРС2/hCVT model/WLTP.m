data = readtable('WLTP.xlsx');

t = data.Time;
v = data.Speed;

velocity = timeseries(v, t);