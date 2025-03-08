struct Propellant
{
    const char* name;
    float density;     // kg/m^3
    float idealTemp;   // Kelvin

    constexpr Propellant(const char* n, float d, float t) : name(n), density(d), idealTemp(t) {}

};

namespace Propellants
{
    constexpr Propellant METHALOX =     { "Liquid Methane/LOX" , 422.0f , 111.0f };
    constexpr Propellant HYDROLOX =     { "Liquid Hydrogen/LOX", 70.0f  , 20.0f  };
    constexpr Propellant KEROSENE =     { "Kerosene (RP-1)/LOX", 820.0f , 298.0f };
    constexpr Propellant SOLID =        { "Solid Propellant"   , 1700.0f, 298.0f };
    constexpr Propellant HYPERGOLIC =   { "Hypergolic"         , 1200.0f, 298.0f };
}

constexpr Propellant ALL_PROPS[] = {
    Propellants::METHALOX,
    Propellants::HYDROLOX,
    Propellants::KEROSENE,
    Propellants::SOLID
};