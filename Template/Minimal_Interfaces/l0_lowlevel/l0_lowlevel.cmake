set(l0_lowlevel_sources "")
set(l0_lowlevel_dirs "")

ess_data(l0_lowlevel_sources
    entry_point.c
)

ess_include(arm)
ess_include(device)
ess_include(newlib)
