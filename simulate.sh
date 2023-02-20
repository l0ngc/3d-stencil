export LD_LIBRARY_PATH=/chalmers/sw/sup64/ansys-2021r1/libpng/lib:${LD_LIBRARY_PATH}
export GEM5_PATH=/chalmers/users/longc/Documents/eda284/lab1/gem5

# name="$1"

# name="s64_basic_mem_access"

name="s16_scalar_c1s8_c2s64_none"

# name="s64_e1a16_c1s8_c2s64_none"

# name="s64_autovec_mem_access"

# name="s64_e1m16_c1s8_c2s64_withauto"

# name="s64_e1m16_c1s8_c2s64_auto"

# name="s64_e1m16_c1s8_c2s64_fuse"

# name="s64_e1m16_c1s8_c2s64_none"

# name="s64_e1m16_c1s8_c2s64_unrollj2"


dir="$GEM5_PATH/m5out/3d_stencil/$name"

stencil_addr="/chalmers/users/longc/Documents/3d-stencil/stencil-3d"

l1d_size="8kB"
l2_size="64kB"

if [ -d "$dir" ]; then
    rm -rf "$dir"
fi

mkdir "$dir"

$GEM5_PATH/build/ARM/gem5.opt -d $dir \
    $GEM5_PATH/configs/example/se.py \
    --interp-dir /usr/aarch64-linux-gnu \
    --redirects /lib=/usr/aarch64-linux-gnu/lib \
    --param 'system.cpu[:].isa[:].sve_vl_se = 16' \
    --cpu-type=ex5_big --caches --l2cache --l1d_size=$l1d_size --l2_size=$l2_size \
    --sys-clock="2GHz" --cpu-clock="2GHz" --mem-type='DDR3_1600_8x8' -c $stencil_addr

# make clean;make;bash simulate.sh