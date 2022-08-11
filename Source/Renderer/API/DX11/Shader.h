#pragma once
#include <sstream>
#include <iostream>
#include <string>

#include "Platform/Platform.h"

namespace SDBX
{
	template<typename ShaderType>
	class Shader
	{
	public:
		Shader<ShaderType>::Shader(ShaderType* m_pShader, ID3DBlob* m_pShaderBlob): m_pShader{ m_pShader }, m_pShaderBlob{ m_pShaderBlob } {}
		~Shader();

		Shader(const Shader&) = delete;
		Shader(Shader&&) noexcept = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) noexcept = delete;

		ShaderType* GetShader() const { return m_pShader; }
		ID3DBlob* GetShaderBlob() const { return m_pShaderBlob; }

	private:
		ShaderType* m_pShader;
		ID3DBlob* m_pShaderBlob;
	};

	template<typename ShaderType>
	Shader<ShaderType>::~Shader()
	{
		SafeRelease(m_pShaderBlob);
		SafeRelease(m_pShader);
	}

	using VertexShader = Shader<ID3D11VertexShader>;
	using HullShader = Shader<ID3D11HullShader>;
	using DomainShader = Shader<ID3D11DomainShader>;
	using GeometryShader = Shader<ID3D11GeometryShader>;
	using PixelShader = Shader<ID3D11PixelShader>;
	using ComputeShader = Shader<ID3D11ComputeShader>;
}