# WOO-006 (#39): build the sample map after woo006_import.py. Run in the editor with -ExecutePythonScript.
# Medieval and Ranger pieces here are blockout placeholders only.
import os
import unreal as U

ROOT = "/Game/ThirdParty/WOO006"
MAP = f"{ROOT}/L_ImportSample"
MEL = U.MaterialEditingLibrary
EAL = U.EditorAssetLibrary
AT = U.AssetToolsHelpers.get_asset_tools()
ES = U.get_editor_subsystem(U.EditorActorSubsystem)
LS = U.get_editor_subsystem(U.LevelEditorSubsystem)
BLEND = U.MaterialProperty


def tex(path, normal=False):
    t = U.load_asset(f"{ROOT}/{path}")
    if normal:
        t.set_editor_property("srgb", False)
        t.set_editor_property("compression_settings", U.TextureCompressionSettings.TC_NORMALMAP)
    return t


def sample(m, t, y, normal=False, tiling=None):
    n = MEL.create_material_expression(m, U.MaterialExpressionTextureSample, -400, y)
    n.set_editor_property("texture", t)
    if normal:
        n.set_editor_property("sampler_type", U.MaterialSamplerType.SAMPLERTYPE_NORMAL)
    if tiling:
        c = MEL.create_material_expression(m, U.MaterialExpressionTextureCoordinate, -700, y)
        c.set_editor_property("u_tiling", tiling)
        c.set_editor_property("v_tiling", tiling)
        MEL.connect_material_expressions(c, "", n, "UVs")
    return n


def mat(name, base, normal=None, orm=None, masked=False, tiling=None, skinned=False):
    path = f"{ROOT}/Materials/{name}"
    if EAL.does_asset_exist(path):
        EAL.delete_asset(path)
    m = AT.create_asset(name, f"{ROOT}/Materials", U.Material, U.MaterialFactoryNew())
    m.set_editor_property("used_with_skeletal_mesh", skinned)
    b = sample(m, tex(base), -300, tiling=tiling)
    MEL.connect_material_property(b, "RGB", BLEND.MP_BASE_COLOR)
    if masked:
        m.set_editor_property("blend_mode", U.BlendMode.BLEND_MASKED)
        m.set_editor_property("two_sided", True)
        MEL.connect_material_property(b, "A", BLEND.MP_OPACITY_MASK)
    if normal:
        MEL.connect_material_property(sample(m, tex(normal, True), 0, True, tiling), "RGB", BLEND.MP_NORMAL)
    if orm:
        o = sample(m, tex(orm), 300)
        MEL.connect_material_property(o, "G", BLEND.MP_ROUGHNESS)
        MEL.connect_material_property(o, "B", BLEND.MP_METALLIC)
    MEL.recompile_material(m)
    EAL.save_asset(path)
    return m


BARK = mat("M_Bark", "Nature/Bark_NormalTree_ncl1_1", "Nature/Bark_NormalTree_Normal")
LEAF = mat("M_TreeLeaves", "Nature/Leaves_NormalTree_C", masked=True)
FERN = mat("M_Fern", "Nature/Textures/Leaves", masked=True)
GRASS = mat("M_Grass", "Nature/Grass_ncl1_1", masked=True)
ROCK = mat("M_Rock", "Nature/Rocks_Diffuse")
GROUND = mat("M_Ground", "Ground/Textures/brown_mud_leaves_01_diff_1k", "Ground/Textures/brown_mud_leaves_01_nor_gl_1k", tiling=12)
W = "Warrior/Textures"
RANGER = mat("M_Ranger", f"{W}/T_Ranger_BaseColor", f"{W}/T_Ranger_Normal", f"{W}/T_Ranger_ORM", skinned=True)
SKIN_R = mat("M_RangerSkin", f"{W}/T_Regular_Male_Dark_BaseColor", f"{W}/T_Regular_Male_Normal", skinned=True)

if EAL.does_asset_exist(MAP):
    EAL.delete_asset(MAP)
LS.new_level(MAP)


def spawn(cls, loc, rot=(0, 0, 0), scale=1.0, label=None):
    a = ES.spawn_actor_from_class(cls, U.Vector(*loc), U.Rotator(roll=rot[0], pitch=rot[1], yaw=rot[2]))
    a.set_actor_scale3d(U.Vector(scale, scale, scale))
    if label:
        a.set_actor_label(label)
    return a


def static(path, loc, scale=1.0, mats=(), yaw=0, label=None):
    a = spawn(U.StaticMeshActor, loc, (0, 0, yaw), scale, label or path.split("/")[-1])
    c = a.static_mesh_component
    c.set_static_mesh(U.load_asset(path))
    for i, m in enumerate(mats):
        c.set_material(i, m)
    return a


def skel(path, anim, loc, yaw, scale=1.0, mats=(), label=None):
    a = spawn(U.SkeletalMeshActor, loc, (0, 0, yaw), scale, label)
    c = a.skeletal_mesh_component
    c.set_skeletal_mesh_asset(U.load_asset(path))
    for i, m in mats:
        c.set_material(i, m)
    c.set_editor_property("animation_mode", U.AnimationMode.ANIMATION_SINGLE_NODE)
    d = c.get_editor_property("animation_data")
    d.set_editor_property("anim_to_play", U.load_asset(anim))
    d.set_editor_property("saved_looping", True)
    d.set_editor_property("saved_playing", True)
    c.set_editor_property("animation_data", d)
    return a


static("/Engine/BasicShapes/Plane", (0, 0, 0), 60, [GROUND], label="Ground")
N, A = f"{ROOT}/Nature", f"{ROOT}/Warrior/Anim/"
static(f"{N}/CommonTree_1", (500, -420, 0), 1.0, [BARK, LEAF])
static(f"{N}/Fern_1", (120, -440, 0), 0.25, [FERN])
static(f"{N}/Grass_Common_Tall", (60, 300, 0), 0.6, [GRASS])
static(f"{N}/Rock_Medium_1", (380, 520, 0), 0.5, [ROCK])

RSKM = f"{ROOT}/Warrior/Male_Ranger"
GRIP = [float(v) for v in os.environ.get("WOO006_GRIP", "0,0,0").split(",")]  # roll, pitch, yaw on hand_r
YAW = 90  # humanoid faces +Y as imported; turn it to the camera
for y, clip, name in [(-170, "Sword_Idle", "Idle"), (120, "Sword_Heavy_Combo", "Heavy")]:
    anim = f"{A}UAL{1 if clip == 'Sword_Idle' else 2}_Standard_Armature_{clip}"
    r = skel(RSKM, anim, (0, y, 0), YAW, 1.0, [(0, RANGER), (1, SKIN_R)], f"Warrior_{name}")
    s = static(f"{ROOT}/Weapons/Claymore", (0, y, 100), 0.24, label=f"Claymore_{name}")
    s.root_component.set_mobility(U.ComponentMobility.MOVABLE)
    s.attach_to_actor(r, "hand_r", U.AttachmentRule.SNAP_TO_TARGET, U.AttachmentRule.SNAP_TO_TARGET, U.AttachmentRule.KEEP_WORLD)
    s.root_component.set_editor_property("relative_rotation", U.Rotator(*GRIP))
    s.set_actor_scale3d(U.Vector(0.24, 0.24, 0.24))

skel(f"{ROOT}/Wolf/Wolf", f"{ROOT}/Wolf/Wolf_Anim_AnimalArmature_Attack", (0, 420, 0), 0, 0.3, label="Wolf_Attack")
skel(f"{ROOT}/Husky/Husky", f"{ROOT}/Husky/Husky_Anim_AnimalArmature_Attack", (0, -560, 0), 0, 0.3, label="Husky_Attack")

sun = spawn(U.DirectionalLight, (0, 0, 1000), (0, -40, 35), label="Sun")
sun.light_component.set_intensity(6.0)
spawn(U.SkyAtmosphere, (0, 0, 0), label="Atmosphere")
sky = spawn(U.SkyLight, (0, 0, 500), label="Sky")
sky.light_component.set_editor_property("real_time_capture", True)
sky.light_component.set_intensity(1.2)
cam = spawn(U.CameraActor, tuple(float(v) for v in os.environ.get("WOO006_CAM", "-640,-60,200").split(",")), (0, -6, 0), label="Cam")
spawn(U.PlayerStart, (-3000, 0, 120), label="Start")  # keep the default pawn out of frame
cam.set_editor_property("auto_activate_for_player", U.AutoReceiveInput.PLAYER0)

for folder in ("Wolf", "Husky", "Warrior"):
    for p in EAL.list_assets(f"{ROOT}/{folder}", recursive=False, include_folder=False):
        a = EAL.load_asset(p)
        if isinstance(a, U.Material):
            a.set_editor_property("used_with_skeletal_mesh", True)
            MEL.recompile_material(a)
            EAL.save_asset(p)
        if isinstance(a, U.SkeletalMesh):
            print(f"WOO006 BOUNDS {p} {a.get_bounds().origin} {a.get_bounds().box_extent}")

LS.save_current_level()

# Keep only the required creature clips.
CLIPS = ("_Idle", "_Walk", "_Gallop", "_Run", "_Attack", "_Death")
for folder in ("Wolf", "Husky"):
    for p in EAL.list_assets(f"{ROOT}/{folder}", recursive=False, include_folder=False):
        pkg = p.split(".")[0]
        if "_Anim_" in pkg and not pkg.endswith(CLIPS):
            EAL.delete_asset(pkg)
print("WOO006 MAP DONE")
U.SystemLibrary.quit_editor()
