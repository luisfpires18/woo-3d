# WOO-014 (#14): build the Warrior movement test arena. Run in the editor:
#   UnrealEditor.exe WeaponsOfOrder.uproject -ExecutePythonScript=Tools/woo014_arena.py
# Geometry uses engine basic shapes only (1 cube = 100 cm). Rebuilding replaces the map.
import unreal as U

MAP = "/Game/WOO014/L_WOO014_Arena"
EAL = U.EditorAssetLibrary
ES = U.get_editor_subsystem(U.EditorActorSubsystem)
LS = U.get_editor_subsystem(U.LevelEditorSubsystem)
CUBE = U.load_asset("/Engine/BasicShapes/Cube")
GROUND = U.load_asset("/Game/ThirdParty/WOO006/Materials/M_Ground")

# Deleting and recreating the map at the same path did not save on a rerun, so clear it instead.
if EAL.does_asset_exist(MAP):
    LS.load_level(MAP)
    OURS = (U.StaticMeshActor, U.DirectionalLight, U.SkyAtmosphere, U.SkyLight, U.PlayerStart)
    ES.destroy_actors([a for a in ES.get_all_level_actors() if isinstance(a, OURS)])  # keep WorldSettings/brush
else:
    LS.new_level(MAP)


def spawn(cls, loc, yaw=0.0, pitch=0.0, label=None):
    a = ES.spawn_actor_from_class(cls, U.Vector(*loc), U.Rotator(roll=0, pitch=pitch, yaw=yaw))
    if label:
        a.set_actor_label(label)
    return a


def box(label, center, size, pitch=0.0, mat=None):
    """Axis-aligned block; center and size in cm."""
    a = spawn(U.StaticMeshActor, center, pitch=pitch, label=label)
    c = a.static_mesh_component
    c.set_static_mesh(CUBE)
    a.set_actor_scale3d(U.Vector(size[0] / 100, size[1] / 100, size[2] / 100))
    if mat:
        c.set_material(0, mat)
    return a


# 30 x 30 m floor, top surface at z = 0.
box("Floor", (0, 0, -25), (3000, 3000, 50), mat=GROUND)
# Perimeter walls, 3 m high.
for label, c, s in [("Wall_N", (1500, 0, 150), (50, 3000, 300)), ("Wall_S", (-1500, 0, 150), (50, 3000, 300)),
                    ("Wall_E", (0, 1500, 150), (3000, 50, 300)), ("Wall_W", (0, -1500, 150), (3000, 50, 300))]:
    box(label, c, s)
# Ledges: 40 cm (step-up height is 45 cm), 90 cm (needs a jump), 200 cm (not reachable).
box("Ledge_40cm", (500, -500, 20), (300, 300, 40))
box("Ledge_90cm", (500, 0, 45), (300, 300, 90))
box("Block_200cm", (500, 500, 100), (300, 300, 200))
# Ramp up to a 1 m platform.
box("Ramp", (-400, 600, 50), (600, 300, 20), pitch=10)
box("Ramp_Top", (0, 600, 45), (300, 300, 90))
# Narrow corridor (2 m wide, 3 m high walls) for camera-collision checks.
box("Corridor_L", (-600, -700, 150), (1200, 30, 300))
box("Corridor_R", (-600, -500, 150), (1200, 30, 300))

sun = spawn(U.DirectionalLight, (0, 0, 1000), pitch=-45, yaw=35, label="Sun")
sun.light_component.set_intensity(6.0)
spawn(U.SkyAtmosphere, (0, 0, 0), label="Atmosphere")
sky = spawn(U.SkyLight, (0, 0, 500), label="Sky")
sky.light_component.set_editor_property("real_time_capture", True)
spawn(U.PlayerStart, (0, 0, 100), label="Start")  # 3.5 m in front of the 90 cm ledge

world = U.get_editor_subsystem(U.UnrealEditorSubsystem).get_editor_world()
world.get_world_settings().set_editor_property("default_game_mode", U.load_class(None, "/Script/WeaponsOfOrder.WooGameMode"))

LS.save_current_level()
print("WOO014 ARENA DONE")
U.SystemLibrary.quit_editor()
