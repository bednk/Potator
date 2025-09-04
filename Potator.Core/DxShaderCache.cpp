#include "DxShaderCache.h"
#include <boost/dll/runtime_symbol_info.hpp>
#include <filesystem>
#include "HlslShader.h"


using namespace Potator;
namespace fs = std::filesystem;

Potator::DxShaderCache::DxShaderCache(std::shared_ptr<IGraphicsDevice> device) : _device { device }
{
    auto shadersDir = boost::dll::program_location().parent_path().string() / fs::path("resources\\shaders");
    for (const auto& entry : fs::directory_iterator(shadersDir))
    {
        if (entry.is_regular_file() && entry.path().extension() != ".cso")
        {
            continue;
        }

        auto name = entry.path().stem().string();
        _binaries[name] = std::make_shared<HlslShader>(entry.path());

        if (name.starts_with("vs_"))
        {
            _vertexShaders[name] = _device->CreateVertexShader(_binaries[name].get());
        }

        if (name.starts_with("ps_"))
        {
            _pixelShaders[name] = device->CreatePixelShader(_binaries[name].get());
        }
    }
}

std::shared_ptr<IShaderBinary> Potator::DxShaderCache::GetShaderBinary(const std::string& name)
{
    return _binaries[name];
}

VertexShaderHandle Potator::DxShaderCache::GetVertexShaderHandle(const std::string& name)
{
    return _vertexShaders[name];
}

PixelShaderHandle Potator::DxShaderCache::GetPixelShaderHandle(const std::string& name)
{
    return _pixelShaders[name];
}
