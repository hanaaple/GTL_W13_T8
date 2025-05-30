import os
import glob
from pathlib import Path

os.chdir(r"C:\Users\Jungle\Desktop\GTL_W13_T8\EngineSIU\EngineSIU\Contents\ParticleMaterials")


current_path = Path(os.getcwd())

dummy_obj = open(current_path / "DummyObj.obj", "w")
dummy_mtl = open(current_path / "ParticleMaterials.mtl", "w")

png_globs = glob.glob(str(current_path / "*.png"), recursive=False)
jpg_globs = glob.glob(str(current_path / "*.jpg"), recursive=False)
image_globs = map(Path, png_globs + jpg_globs)


mtl_list: list[str] = []
dummy_obj.write("mtllib ParticleMaterials.mtl\n\n")
for f in image_globs:
    dummy_obj.write(f"usemtl {f.stem}\n")
    mtl_list.append(f"newmtl {f.stem}\nmap_Kd {f.name}\n")

dummy_mtl.write("\n".join(mtl_list))

dummy_obj.close()
dummy_mtl.close()
