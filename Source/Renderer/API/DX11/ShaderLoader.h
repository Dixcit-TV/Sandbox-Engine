#pragma once

#include "Platform/Platform.h"
#include "Renderer/API/DX11/Shader.h"
#include "Resources/Loaders/ILoader.h"

namespace SDBX
{
	template<typename ShaderType>
	class ShaderLoader final : public Resource::IBaseLoader<ShaderLoader>
	{
		using ShaderCreationFnc = HRESULT(__stdcall ID3D11Device::*)(const void*, SIZE_T, ID3D11ClassLinkage*, SHADER_TYPE**);
	public:
		explicit ShaderLoader() = default;
		ShaderLoader(const ShaderLoader&) = delete;
		ShaderLoader(ShaderLoader&&) noexcept = delete;
		ShaderLoader& operator=(const ShaderLoader&) = delete;
		ShaderLoader& operator=(ShaderLoader&&) noexcept = delete;
		~ShaderLoader() override = default;

		Shader<ShaderType>* LoadContent(const std::wstring& path, ID3D11Device* pdevice, const char* entryPoint) const override;
	};
	
	template<typename ShaderType>
	Shader<ShaderType>* ShaderLoader<ShaderType>::LoadContent(const std::wstring& path, ID3D11Device* pdevice, const char* entryPoint) const
	{
		Shader<ShaderType>* shader{ new Shader<ShaderType>() };

		ID3DBlob* perrorBlob{ nullptr };

		UINT flags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif 

		HRESULT res{ E_INVALIDARG };

		std::string target{};
		ShaderCreationFnc shaderFnc{};

		if constexpr (std::is_same_v<ShaderType, ID3D11VertexShader>)
		{
			target = "vs_5_0";
			shaderFnc = &ID3D11Device::CreateVertexShader;
		}
		else if constexpr (std::is_same_v<ShaderType, ID3D11HullShader>)
		{
			target = "hs_5_0";
			shaderFnc = &ID3D11Device::CreateHullShader;
		}
		else if constexpr (std::is_same_v<ShaderType, ID3D11DomainShader>)
		{
			target = "ds_5_0";
			shaderFnc = &ID3D11Device::CreateDomainShader;
		}
		else if constexpr (std::is_same_v<ShaderType, ID3D11GeometryShader>)
		{
			target = "gs_5_0";
			shaderFnc = &ID3D11Device::CreateGeometryShader;
		}
		else if constexpr (std::is_same_v<ShaderType, ID3D11PixelShader>)
		{
			target = "ps_5_0";
			shaderFnc = &ID3D11Device::CreatePixelShader;
		}
		else if constexpr (std::is_same_v<ShaderType, ID3D11ComputeShader>)
		{
			target = "cs_5_0";
			shaderFnc = &ID3D11Device::CreateComputeShader;
		}
		else
		{
			return res;
		}

		res = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target.c_str(), flags, 0, &shader->GetShaderBlob(), &perrorBlob);
	
		if (FAILED(res))
		{
			std::wstringstream ss;
			ss << L"ShaderLoader: Failed to CreateEffectFromFile! Path: " << path;

			if (perrorBlob != nullptr)
			{
				wchar_t* errors = static_cast<wchar_t*>(perrorBlob->GetBufferPointer());
				const std::wstring errorString{ errors, perrorBlob->GetBufferSize() };
				ss << L", " << errorString;
				SafeRelease(perrorBlob);
			}

			SDBX_W_LOG(WARNING_LOG, ss.str())
			return res;
		}

		res = (pdevice->*shaderFnc)(shader->GetShaderBlob()->GetBufferPointer(), shader->GetShaderBlob()->GetBufferSize(), NULL, &shader->GetShader());
		SDBX_W_ASSERT_AS_WARNING(SUCCEEDED(res), L"Could not create shader: " + path)

		return shader;
	}
}
