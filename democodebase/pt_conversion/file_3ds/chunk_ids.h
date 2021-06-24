#ifndef PT_CONVERSION_FILE_3DS_CHUNK_IDS_H
#define PT_CONVERSION_FILE_3DS_CHUNK_IDS_H

namespace pt { namespace conversion { namespace file_3ds {

enum CHUNK_TYPE {
    CHUNK_RGBF      = 0x0010,
    CHUNK_RGBB      = 0x0011,
    CHUNK_RGBB_GAMMA_CORRECTED = 0x0012,
    CHUNK_RGBF_GAMMA_CORRECTED = 0x0013,

    CHUNK_PERCENTI = 0x0030,
    CHUNK_PERCENTF = 0x0031,

    CHUNK_PRJ       = 0xC23D,
    CHUNK_MLI       = 0x3DAA,

    CHUNK_MAIN      = 0x4D4D,
        CHUNK_OBJMESH   = 0x3D3D,
            CHUNK_ONE_UNIT = 0x0100,
            CHUNK_BKGCOLOR  = 0x1200,
            CHUNK_AMBCOLOR  = 0x2100,
            CHUNK_OBJBLOCK  = 0x4000,
                CHUNK_TRIMESH   = 0x4100,
                    CHUNK_VERTLIST  = 0x4110,
                    CHUNK_VERTEX_FLAG_LIST = 0x4111, // short nflags; short flags[nflags];
                    CHUNK_FACELIST  = 0x4120,
                        CHUNK_FACEMAT   = 0x4130,
                        CHUNK_SMOOLIST  = 0x4150, // short grouplist[n]; determined by length, seems to be 4 per face
                    CHUNK_MAPLIST   = 0x4140,
                    CHUNK_TRMATRIX  = 0x4160,
					CHUNK_MESHCOLOR = 0x4165,
					CHUNK_MESHTEXTUREINFO = 0x4170,
                CHUNK_LIGHT     = 0x4600,
                    CHUNK_SPOTLIGHT = 0x4610,
                        CHUNK_SPOTLIGHT_MAP = 0x4653,
                    CHUNK_ATTENUATION_ON = 0x4625,
                    CHUNK_RANGE_START = 0x4659,
                    CHUNK_RANGE_END = 0x465A,
                    CHUNK_MULTIPLIER = 0x465B,
                CHUNK_CAMERA    = 0x4700,
                CHUNK_HIERARCHY = 0x4F00,
        CHUNK_VIEWPORT  = 0x7001,
        CHUNK_MATERIAL  = 0xAFFF,
            CHUNK_MATNAME   = 0xA000,
            CHUNK_AMBIENT   = 0xA010,
            CHUNK_DIFFUSE   = 0xA020,
            CHUNK_SPECULAR  = 0xA030,
            CHUNK_SHININESS_PERCENT             = 0xA040,
            CHUNK_SHININESS_STRENGTH_PERCENT    = 0xA041,
            CHUNK_TRANSPARENCY_PERCENT          = 0xA050,
            CHUNK_TRANSPARENCY_FALLOFF_PERCENT  = 0xA052,
            CHUNK_REFLECTION_BLUR_PERCENT       = 0xA053,
            CHUNK_TWO_SIDED = 0xA081,
            CHUNK_ADD_TRANS = 0xA083,
            CHUNK_SELF_ILLUM = 0xA084,
            CHUNK_WIRE_FRAME_ON = 0xA085,
            CHUNK_WIRE_THICKNESS = 0xA087,
            CHUNK_FACE_MAP = 0xA088,
            CHUNK_IN_TRANC = 0xA08A,
            CHUNK_SOFTEN = 0xA08C,
            CHUNK_WIRE_IN_UNITS = 0xA08E,
            CHUNK_RENDER_TYPE = 0xA100,
            CHUNK_TRANSPARENCY_FALLOFF_PERCENT_PRESENT = 0xA240,
            CHUNK_REFLECTION_BLUR_PERCENT_PRESENT = 0xA250,
            CHUNK_BUMP_MAP_PRESENT = 0xA252,

            CHUNK_TEXTURE_MAP_1 = 0xA200,
            CHUNK_TEXTURE_MAP_2 = 0xA33A,
            CHUNK_OPACITY_MAP = 0xA210,
            CHUNK_BUMP_MAP = 0xA230,
            CHUNK_SHININESS_MAP = 0xA33C,
            CHUNK_SPECULAR_MAP = 0xA204,
            CHUNK_SELF_ILLUM_MAP = 0xA33D,
            CHUNK_REFLECTION_MAP = 0xA220,
            CHUNK_MASK_FOR_TEXTURE_MAP_1 = 0xA33E,
            CHUNK_MASK_FOR_TEXTURE_MAP_2 = 0xA340,
            CHUNK_MASK_FOR_OPACITY_MAP = 0xA342,
            CHUNK_MASK_FOR_BUMP_MAP = 0xA344,
            CHUNK_MASK_FOR_SHININESS_MAP = 0xA346,
            CHUNK_MASK_FOR_SPECULAR_MAP = 0xA348,
            CHUNK_MASK_FOR_SELF_ILLUM_MAP = 0xA34A,
            CHUNK_MASK_FOR_REFLECTION_MAP = 0xA34C,
            // Sub-chunks for all maps:
                CHUNK_MAPPING_FILENAME = 0xA300,
                CHUNK_MAPPING_PARAMETERS = 0xA351,
                CHUNK_BLUR_PERCENT = 0xA353,
                CHUNK_V_SCALE = 0xA354,
                CHUNK_U_SCALE = 0xA356,
                CHUNK_U_OFFSET = 0xA358,
                CHUNK_V_OFFSET = 0xA35A,
                CHUNK_ROTATION_ANGLE = 0xA35C,
                CHUNK_RGB_LUMA_OR_ALPHA_TINT_1 = 0xA360,
                CHUNK_RGB_LUMA_OR_ALPHA_TINT_2 = 0xA362,
                CHUNK_RGB_TINT_R = 0xA364,
                CHUNK_RGB_TINT_G = 0xA366,
                CHUNK_RGB_TINT_B = 0xA368,

        CHUNK_KEYFRAMER = 0xB000,
            CHUNK_KF_AMBIENT_INFO           = 0xB001,
            CHUNK_KF_MESH_INFO              = 0xB002,
            CHUNK_KF_CAMERA_INFO            = 0xB003,
            CHUNK_KF_CAMERA_TARGET_INFO     = 0xB004,
            CHUNK_KF_OMNI_LIGHT_INFO        = 0xB005,
            CHUNK_KF_SPOTLIGHT_TARGET_INFO  = 0xB006,
            CHUNK_KF_SPOTLIGHT_INFO         = 0xB007,
            CHUNK_FRAMES                    = 0xB008,
            // subchunks for 0xB001 -- 0xB007
                CHUNK_KF_OBJECT_NAME_PARAMETERS_AND_HIERARCHY_FATHER = 0xB010,
                CHUNK_KF_OBJECT_PIVOT_POINT    = 0xB013,
                CHUNK_KF_OBJECT_MORPH_ANGLE    = 0xB015,
                CHUNK_KF_POSITION_TRACK        = 0xB020,
                CHUNK_KF_ROTATION_TRACK        = 0xB021,
                CHUNK_KF_SCALE_TRACK           = 0xB022,
                CHUNK_KF_FOV_TRACK             = 0xB023,
                CHUNK_KF_ROLL_TRACK            = 0xB024,
                CHUNK_KF_COLOR_TRACK           = 0xB025,
                CHUNK_KF_MORPH_TRACK           = 0xB026,
                CHUNK_KF_HOTSPOT_TRACK         = 0xB027,
                CHUNK_KF_FALLOFF_TRACK         = 0xB028,
                CHUNK_KF_HIDE_TRACK            = 0xB029,
                CHUNK_KF_HIERARCHY_POSITION    = 0xB030,
};

}}} // namespace pt::conversion::file_3ds

#endif
