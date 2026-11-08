# WOO-006 (#39): import the minimal CC0 sample. Run by UnrealEditor-Cmd -run=pythonscript.
# Source folder comes from env WOO006_SRC (extracted files, outside the repo).
import os
import unreal as U

SRC = os.environ["WOO006_SRC"]
ROOT = "/Game/ThirdParty/WOO006"
KEEP_ANIMS = {"Idle_Loop", "Walk_Loop", "Jog_Fwd_Loop", "Sword_Idle", "Sword_Attack", "Death01",
              "Sword_Regular_A", "Sword_Heavy_Combo"}
AT = U.AssetToolsHelpers.get_asset_tools()
EAL = U.EditorAssetLibrary

U.SystemLibrary.execute_console_command(None, "Interchange.FeatureFlags.Import.FBX false")


def task(path, dest, options=None):
    t = U.AssetImportTask()
    t.filename = os.path.join(SRC, path)
    t.destination_path = f"{ROOT}/{dest}"
    t.automated = True
    t.replace_existing = True
    t.save = False
    if options:
        t.options = options
    AT.import_asset_tasks([t])
    paths = list(t.imported_object_paths)
    print(f"WOO006 IMPORT {path} -> {paths}")
    if not paths:
        raise RuntimeError(f"nothing imported from {path}")
    return paths


def ui(kind, anims=False, skeleton=None, mesh=True, scale=1.0):
    o = U.FbxImportUI()
    o.skeletal_mesh_import_data.import_uniform_scale = scale
    o.anim_sequence_import_data.import_uniform_scale = scale
    o.import_mesh = mesh
    o.import_materials = mesh
    o.import_textures = False
    o.import_animations = anims
    o.automated_import_should_detect_type = False
    o.mesh_type_to_import = kind
    o.original_import_type = kind
    if kind == U.FBXImportType.FBXIT_SKELETAL_MESH:
        o.import_as_skeletal = True
        if skeleton:
            o.skeleton = skeleton
        if not mesh:
            o.original_import_type = U.FBXImportType.FBXIT_ANIMATION
            o.mesh_type_to_import = U.FBXImportType.FBXIT_ANIMATION
    else:
        o.static_mesh_import_data.auto_generate_collision = True
        o.static_mesh_import_data.combine_meshes = True
    return o


SM, SK = U.FBXImportType.FBXIT_STATIC_MESH, U.FBXImportType.FBXIT_SKELETAL_MESH

for f in ["CommonTree_1", "Fern_1", "Grass_Common_Tall", "Rock_Medium_1"]:
    task(f"nature/{f}.fbx", "Nature", ui(SM))
task("weapons/Claymore.fbx", "Weapons", ui(SM))

for png in ["Leaves"]:  # the other nature textures arrive with the FBX materials
    task(f"nature/{png}.png", "Nature/Textures")
for png in ["T_Ranger_BaseColor", "T_Ranger_Normal", "T_Ranger_ORM"]:
    task(f"outfit/{png}.png", "Warrior/Textures")
for png in ["T_Regular_Male_Dark_BaseColor", "T_Regular_Male_Normal"]:
    task(f"outfit/{png}.png", "Warrior/Textures")
for jpg in ["brown_mud_leaves_01_diff_1k", "brown_mud_leaves_01_nor_gl_1k"]:
    task(f"polyhaven/{jpg}.jpg", "Ground/Textures")

HUMAN = 100.0  # humanoid packs export in metres
task("outfit/Male_Ranger.fbx", "Warrior", ui(SK, scale=HUMAN))
skel = U.load_asset(f"{ROOT}/Warrior/Male_Ranger_Skeleton")
task("animals/Wolf.fbx", "Wolf", ui(SK, anims=True))
for f in ["ual1/UAL1_Standard.fbx", "ual2/UAL2_Standard.fbx"]:
    task(f, "Warrior/Anim", ui(SK, anims=True, skeleton=skel, mesh=False, scale=HUMAN))
for p in EAL.list_assets(f"{ROOT}/Warrior/Anim", recursive=True, include_folder=False):
    if p.split(".")[0].split("Armature_")[-1] not in KEEP_ANIMS:
        EAL.delete_asset(p)

for p in EAL.list_assets(ROOT, recursive=True, include_folder=False):
    a = EAL.load_asset(p)
    extra = ""
    if isinstance(a, U.StaticMesh):
        b = a.get_bounding_box()
        extra = f"size={(b.max - b.min)} slots={[str(m.material_slot_name) for m in a.static_materials]} bodysetup={a.get_editor_property('body_setup') is not None}"
    elif isinstance(a, U.SkeletalMesh):
        b = a.get_bounds()
        extra = f"extent={b.box_extent} slots={[str(m.material_slot_name) for m in a.materials]} skel={a.skeleton.get_path_name()}"
    elif isinstance(a, U.AnimSequence):
        extra = f"len={a.get_play_length():.2f} skel={a.get_editor_property('skeleton').get_name()}"
    print(f"WOO006 ASSET {type(a).__name__} {p} {extra}")

EAL.save_directory(ROOT, only_if_is_dirty=False)
print("WOO006 IMPORT DONE")
