# odvii

This tool was created for use with SWARM.

https://github.com/MaynardMiner/SWARM

You are welcome to use this software, and distribute FREE at not cost.

odvii - command line tool for windows AMD ADL.

To use:

odvii.exe s

Will produce current stats for all your AMD GPU's

```
Gpu 0 Name= Radeon RX Vega
Gpu 0 RPM=2700
Gpu 0 Fan=82
Gpu 0 Watts=229
Gpu 0 Temp=70
Gpu 1 Name= Radeon RX 580 Series
Gpu 1 RPM=2056
Gpu 1 Fan=59
Gpu 1 Watts=136
Gpu 1 Temp=62
```

RPM is fan rpm.

Fan is % speed reading.

This allows fast and easy parsing for stats- particularly through Powershell:

```
> invoke-expression ".\odvii.exe s" | Tee-Object -Variable stats | Out-Null
> $stats | ConvertFrom-StringData
> Name                           Value
> ----                           -----
> Gpu 0 Name                     Radeon RX Vega
> Gpu 0 RPM                      3754
> Gpu 0 Fan                      114
> Gpu 0 Watts                    190
> Gpu 0 Temp                     70
> Gpu 1 Name                     Radeon RX 580 Series
> Gpu 1 RPM                      2065
> Gpu 1 Fan                      59
> Gpu 1 Watts                    137
> Gpu 1 Temp                     69
```

GPU numbers are based on AMD cards, so GPU 0 would be first AMD card (in lowed Bus #)

Known issue:

Some manufacturers set max fan settings incorrectly. This can result in higher than normal fan percentage,
and "Fan" is calculated based on Maximum RPM and Current RPM.
