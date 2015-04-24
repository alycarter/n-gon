#include "GraphicsManager.h"
#include "Window.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "InputLayoutReader.h"
#include "RenderPass.h"
#include <D3Dcompiler.h>
#include <wincodec.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

GraphicsManager::GraphicsManager()
{
	swapchain = NULL;
	dev = NULL;
	devcon = NULL;
	backBuffer = NULL;
	depthBuffer = NULL;
	vertexBuffer = NULL;
	constantBuffer = NULL;
	indexBuffer = NULL;
	instanceBuffer = NULL;
	textureResource = NULL;
	textureShaderView = NULL;
	factory = NULL;
	screenResolution.x = 0;
	screenResolution.y = 0;
	textureResolution.x = 0;
	textureResolution.y = 0;
	textureBufferSize = 0;
	constantBufferSize = 0;
	instanceBufferSize = 0;
	vertexBufferSize = 0;
	indexBufferSize = 0;
	boundShader = UINT_MAX;
	boundInstanceStride = 0;
}

GraphicsManager::~GraphicsManager()
{

}

void GraphicsManager::create(Window * window, unsigned int textureWidth, unsigned int textureHeight)
{
	textureResolution.x = (float)textureWidth;
	textureResolution.y = (float)textureHeight;
	//set up the directx contexts
	createDirectXContext(window);
	//create the render buffers
	createRenderTarget();
	createDepthBuffer();
	//set up the viewport
	setUpViewPort();
	printf("initial buffer creation \n");
	resizeConstantBuffer(16);
	resizeIndexBuffer(16);
	resizeInstanceBuffer(16);
	resizeTextureArray(16);
	resizeVertexBuffer(16);
	//initialize the thing that makes the factory
	CoInitialize(NULL);
	//create the imaging factory
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (LPVOID *)&factory);
}

void GraphicsManager::createDirectXContext(Window * window)
{
	//set the resolution of the window
	screenResolution = XMFLOAT2((float)window->getWidth(), (float)window->getHeight());
	// create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = window->getWindowHandle();           // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    // create a device, device context and swap chain using the information in the scd struct
	
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);
	swapchain->ResizeBuffers(1, (UINT)screenResolution.x, (UINT)screenResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	// select which primtive type we are using
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GraphicsManager::swapBuffers()
{
	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

XMFLOAT2 * GraphicsManager::getScreenResolution()
{
	return &screenResolution;
}

void GraphicsManager::createRenderTarget()
{
	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();
	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);
}

void GraphicsManager::setUpViewPort()
{
	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = screenResolution.x;
	viewport.Height = screenResolution.y;
	viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0
	devcon->RSSetViewports(1, &viewport);
}

void GraphicsManager::clearRenderBuffer()
{
	//clear the current render buffer
	devcon->ClearRenderTargetView(backBuffer, (float *)&clearColor);
}

void GraphicsManager::clearDepthBuffer()
{
	//clear the current depth buffer
	devcon->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GraphicsManager::createDepthBuffer()
{
	// create the depth buffer texture
	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));
	texd.Width = (UINT)screenResolution.x;
	texd.Height = (UINT)screenResolution.y;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 4;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ID3D11Texture2D *pDepthBuffer;
	dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);
	// create the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &depthBuffer);
	pDepthBuffer->Release();
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// Stencil test parameters
	dsDesc.StencilEnable = false;
	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	dev->CreateDepthStencilState(&dsDesc, &pDSState);
	devcon->OMSetDepthStencilState(pDSState, 1);
	devcon->OMSetRenderTargets(1, &backBuffer, depthBuffer);
}

void GraphicsManager::setClearColor(XMFLOAT4 * color)
{
	clearColor = *color;
}
unsigned int GraphicsManager::getGraphicsResourceID(RESOURCE_TYPE resourceType, char * resourceName)
{
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == resourceType && strcmp(graphicsResources.at(i)->getName(), resourceName) == 0)
		{
			return i;
		}
	}
	printf(resourceName);
	printf(" not found \n");
	return 0;
}

unsigned int GraphicsManager::getTextureID(unsigned int textureResourceID)
{
	if (graphicsResources.at(textureResourceID)->getType() == RESOURCE_TYPE_TEXTURE)
	{
		return ((Texture*)graphicsResources.at(textureResourceID))->getTextureIndex();
	} 
	printf(graphicsResources.at(textureResourceID)->getName());
	printf(" is not a texture \n");
	return 0;
}

void GraphicsManager::getMeshBoundingSphere(unsigned int meshID, BoundingSphere * sphere)
{
	if (graphicsResources.at(meshID)->getType() == RESOURCE_TYPE_MESH)
	{
		((Mesh*)graphicsResources.at(meshID))->getBoundingSphere(sphere);
	} else
	{
		printf(graphicsResources.at(meshID)->getName());
		printf(" is not a mesh \n");
	}
}

void GraphicsManager::bindShader(unsigned int shaderResourceID)
{
	if (boundShader != shaderResourceID)
	{
		GraphicsResource * resource = graphicsResources.at(shaderResourceID);
		if (resource->getType() == RESOURCE_TYPE_SHADER)
		{
			Shader * shader = (Shader*)resource;
			//set the vertex shader
			devcon->VSSetShader(shader->getVertexShader(), 0, 0);
			//set the pixel shader
			devcon->PSSetShader(shader->getPixelShader(), 0, 0);
			devcon->IASetInputLayout(shader->getInput());
			boundShader = shaderResourceID;
		}
		else
		{
			printf(resource->getName());
			printf(" is not a shader \n");
		}
	}
}

unsigned int GraphicsManager::createMeshResourceFromFile(char * resourceName, char * file, State * boundState)
{
	printf("loading mesh: ");
	printf(resourceName);
	printf("\n");
	ObjLoader ldr;
	Mesh * mesh = new Mesh();
	ldr.loadObj(file, mesh->getVerticies(), mesh->getIndicies(), true);
	mesh->calculateBoundingSphere();
	unsigned int id = getFreeGraphicsResourceSlot();
	fitMeshInFreeSpace(mesh);
	mesh->setName(resourceName);
	mesh->bindToState(boundState);
	graphicsResources.at(id) = mesh;
	return id;
}

unsigned int GraphicsManager::createTextureResourceFromFile(char * resourceName, LPWSTR file, State * boundState)
{
	printf("loading texture: ");
	printf(resourceName);
	printf("\n");
	//create the decoder from the factory
	IWICBitmapDecoder * decoder;
	factory->CreateDecoderFromFilename(file, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	//get the frame from the decoder
	IWICBitmapFrameDecode * frame;
	decoder->GetFrame(0, &frame);
	//our textures will use 4 channels rgba
	UINT channels = 4;
	//bytes in one row
	UINT stride = (int)textureResolution.x * channels;
	//bytes in one texture
	UINT size = stride * (int)textureResolution.y;
	BYTE * textureData = new BYTE[size];
	//create a scaler from the frame so we can fit it to the resource size
	IWICBitmapScaler * scalar;
	factory->CreateBitmapScaler(&scalar);
	scalar->Initialize(frame, (int)textureResolution.x, (int)textureResolution.y, WICBitmapInterpolationModeNearestNeighbor);
	//get the pixels that make up our texture
	scalar->CopyPixels(NULL, stride, size, textureData);
	//clean up file loaders
	frame->Release();
	decoder->Release();
	unsigned int id = getFreeGraphicsResourceSlot();
	Texture * texture = new Texture();
	texture->setTexture(getFreeTextureSlot(), (char *)textureData, resourceName);
	texture->bindToState(boundState);
	graphicsResources.at(id) = texture;
	return id;
}

void GraphicsManager::writeTexture(Texture * texture)
{
	//our textures will use 4 channels rgba
	UINT channels = 4;
	//bytes in one row
	UINT stride = (int)textureResolution.x * channels;
	//bytes in one texture
	UINT size = stride * (int)textureResolution.y;
	//put our texture into the subresourse in the texture buffer
	devcon->UpdateSubresource(textureResource, texture->getTextureIndex(), NULL, texture->getTextureData(), stride, size);
}

void GraphicsManager::writeMesh(Mesh * mesh)
{
	D3D11_BOX box = { 0, 0, 0, 0, 1, 1 };
	//copy the vertex data into the buffer
	box.left = mesh->getVertexOffset() * 8 * sizeof(float);
	box.right = box.left + (mesh->getVerticies()->size() * sizeof(float));
	devcon->UpdateSubresource(vertexBuffer, 0, &box, &mesh->getVerticies()->at(0), 0, 0);
	//copy our index data into the buffer
	box.left = mesh->getIndexOffset() * sizeof(unsigned int);
	box.right = box.left + (mesh->getIndicies()->size() * sizeof(unsigned int));
	devcon->UpdateSubresource(indexBuffer, 0, &box, &mesh->getIndicies()->at(0), 0, 0);
}

unsigned int GraphicsManager::createShaderResourceFromFile(char * resourceName, LPWSTR file, char * fileName, State * boundState)
{
	printf("loading shader: ");
	printf(resourceName);
	printf("\n");
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DCompileFromFile(file, NULL, NULL, "VShader", "vs_4_0", 0, 0, &VS, NULL);
	D3DCompileFromFile(file, NULL, NULL, "PShader", "ps_4_0", 0, 0, &PS, NULL);
	ID3D11VertexShader * pVS;
	ID3D11PixelShader * pPS;
	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	// read layouts
	InputLayoutReader rdr;
	vector<D3D11_INPUT_ELEMENT_DESC> layout;
	rdr.parseInputLayout(fileName, &layout);
	ID3D11InputLayout * input;
	dev->CreateInputLayout(&layout.at(0), layout.size(), VS->GetBufferPointer(), VS->GetBufferSize(), &input);
	//create a new shader object and add our shaders to it
	Shader * shader = new Shader();
	shader->setShader(pVS, pPS, input, resourceName);
	shader->bindToState(boundState);
	//add our shader to the list of shaders
	unsigned int id = getFreeGraphicsResourceSlot();
	graphicsResources.at(id) = shader;
	//release the blob objects
	VS->Release();
	PS->Release();
	//return the shader id
	return id;
}

void GraphicsManager::setConstantBufferData(void * data, unsigned int size)
{
	//resize the constant buffer if it doesnt fit
	while (size > constantBufferSize)
	{
		resizeConstantBuffer(size);
	}
	//map to the constant buffer so that we overwrite
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(constantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	//copy the new data into the buffer
	memcpy(ms.pData, data, size);
	//un map the buffer
	devcon->Unmap(constantBuffer, NULL);
}

void GraphicsManager::setInstanceBufferData(void * data, unsigned int size, unsigned int stride)
{
	//resize the instance buffer if it doesnt fit
	while (size > instanceBufferSize)
	{
		//add a little extra to add growing room
		resizeInstanceBuffer(size + (size - instanceBufferSize));
	}
	//map to the instance buffer so that we overwrite
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(instanceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	//copy the new data into the buffer
	memcpy(ms.pData, data, size);
	//un map the buffer
	devcon->Unmap(instanceBuffer, NULL);
	if (boundInstanceStride != stride)
	{
		UINT offset = 0;
		devcon->IASetVertexBuffers(1, 1, &instanceBuffer, &stride, &offset);
		boundInstanceStride = stride;
	}
}

void GraphicsManager::drawMesh(unsigned int meshResourceID, unsigned int count)
{
	GraphicsResource * resource = graphicsResources.at(meshResourceID);
	//if the mesh id is valid
	if (resource->getType() == RESOURCE_TYPE_MESH)
	{
		//get information about the mesh
		Mesh * mesh = (Mesh*)resource;
		// draw the mesh with the data in the instance buffer if its a used id
		devcon->DrawIndexedInstanced(mesh->getIndexCount(), count, mesh->getIndexOffset(), mesh->getVertexOffset(), 0);
	}
	else
	{
		printf(resource->getName());
		printf(" is not a mesh \n");
	}
}


unsigned int GraphicsManager::getFreeGraphicsResourceSlot()
{
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) == NULL)
		{
			return i;
		}
	}
	graphicsResources.push_back(NULL);
	return graphicsResources.size() - 1;
}

unsigned int GraphicsManager::getFreeTextureSlot()
{
	vector<Texture*> textures;
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == RESOURCE_TYPE_TEXTURE)
		{
			textures.push_back((Texture*)graphicsResources.at(i));
		}
	}
	unsigned int testSlot = 0;
	while (testSlot < textureBufferSize)
	{
		bool filled = false;
		for (unsigned int i = 0; i < textures.size() && !filled; i++)
		{
			if (textures.at(i)->getTextureIndex() == testSlot)
			{
				filled = true;
			}
		}
		if (!filled)
		{
			return testSlot;
		}
		testSlot++;
	}
	resizeTextureArray(textureBufferSize * 2);
	return testSlot;
}

void GraphicsManager::fitMeshInFreeSpace(Mesh * mesh)
{
	fitMeshInIndexSpace(mesh);
	fitMeshInVertexSpace(mesh);
}

void GraphicsManager::fitMeshInVertexSpace(Mesh * mesh)
{
	vector<Mesh*> meshes;
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == RESOURCE_TYPE_MESH)
		{
			meshes.push_back((Mesh*)graphicsResources.at(i));
		}
	}
	unsigned int start = 0;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if ((meshes.at(i)->getVertexOffset() - start) < mesh->getVertexCount())
		{
			start = meshes.at(i)->getVertexOffset() + meshes.at(i)->getVertexCount();
			meshes.erase(meshes.begin() + i);
			i = -1;
		}
	}
	if (vertexBufferSize < ((start + mesh->getVertexCount()) * sizeof(float) * 8))
	{
		resizeVertexBuffer((start + mesh->getVertexCount()) * sizeof(float) * 8);
	}
	mesh->setVertexOffset(start);
}

void GraphicsManager::fitMeshInIndexSpace(Mesh * mesh)
{
	vector<Mesh*> meshes;
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == RESOURCE_TYPE_MESH)
		{
			meshes.push_back((Mesh*)graphicsResources.at(i));
		}
	}
	unsigned int start = 0;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if ((meshes.at(i)->getIndexOffset() - start) < mesh->getIndexCount())
		{
			start = meshes.at(i)->getIndexOffset() + meshes.at(i)->getIndexCount();
			meshes.erase(meshes.begin() + i);
			i = -1;
		}
	}
	if (indexBufferSize < ((start + mesh->getIndexCount()) * sizeof(unsigned int)))
	{
		resizeIndexBuffer((start + mesh->getIndexCount()) * sizeof(unsigned int));
	}
	mesh->setIndexOffset(start);
}

void GraphicsManager::resizeTextureArray(unsigned int minimumSize)
{
	textureBufferSize = minimumSize;
	printf("resizing texture buffer to fit : %u textures\n", constantBufferSize);
	if (textureResource != NULL)
	{
		textureResource->Release();
		textureShaderView->Release();
	}
	UINT channels = 4;
	//stide is how many bytes in a row
	UINT stride = (int)textureResolution.x * channels;
	//size is how many bytes in a texture
	UINT size = stride * (int)textureResolution.y;
	//create the subresource data array to store the info about each texture
	BYTE * textureData = new BYTE[size];
	//create the subresource data
	D3D11_SUBRESOURCE_DATA * subData = new D3D11_SUBRESOURCE_DATA[textureBufferSize];
	for (unsigned int i = 0; i < textureBufferSize; i++)
	{
		subData[i].pSysMem = textureData;
		subData[i].SysMemPitch = stride;
		subData[i].SysMemSlicePitch = size;
	}
	//create the texture array
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = (int)textureResolution.x;
	desc.Height = (int)textureResolution.y;
	desc.MipLevels = 1;
	desc.ArraySize = textureBufferSize;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	//create the texture 2d buffer
	HRESULT res = dev->CreateTexture2D(&desc, subData, &textureResource);
	//set up the shader view resourse as a texture 2d array
	D3D11_SHADER_RESOURCE_VIEW_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	sdesc.Texture2DArray.ArraySize = textureBufferSize;
	sdesc.Texture2DArray.FirstArraySlice = 0;
	sdesc.Texture2DArray.MipLevels = 1;
	sdesc.Texture2DArray.MostDetailedMip = 0;
	sdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	//link the shader resource to the texture
	dev->CreateShaderResourceView((ID3D11Resource *)textureResource, &sdesc, &textureShaderView);
	//clean up data
	delete[] textureData;
	delete[] subData;
	//rewrite the existing texture data
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == RESOURCE_TYPE_TEXTURE)
		{
			writeTexture((Texture*)graphicsResources.at(i));
		}
	}
	devcon->PSSetShaderResources(0, 1, &textureShaderView);
}

void GraphicsManager::resizeConstantBuffer(unsigned int minimumSize)
{
	constantBufferSize = minimumSize + (16 - (minimumSize % 16));
	printf("resizing constant buffer to fit : %u bytes \n", constantBufferSize);
	if (constantBuffer != NULL)
	{
		constantBuffer->Release();
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = constantBufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dev->CreateBuffer(&bd, NULL, &constantBuffer);
	devcon->VSSetConstantBuffers(0, 1, &constantBuffer);
	devcon->PSSetConstantBuffers(0, 1, &constantBuffer);
}

void GraphicsManager::resizeInstanceBuffer(unsigned int minimumSize)
{
	instanceBufferSize = minimumSize;
	printf("resizsing instance buffer to fit : %u bytes \n", instanceBufferSize);
	if (instanceBuffer != NULL)
	{
		instanceBuffer->Release();
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = instanceBufferSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dev->CreateBuffer(&bd, NULL, &instanceBuffer);
	boundInstanceStride = 0;
}

void GraphicsManager::resizeVertexBuffer(unsigned int minimumSize)
{
	vertexBufferSize = minimumSize;
	printf("resizsing vertex buffer to fit : %u bytes \n", vertexBufferSize);
	if (vertexBuffer != NULL)
	{
		vertexBuffer->Release();
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexBufferSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dev->CreateBuffer(&bd, NULL, &vertexBuffer);
	UINT offset = 0;
	UINT stride = 8 * sizeof(float);
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == RESOURCE_TYPE_MESH)
		{
			writeMesh((Mesh*)graphicsResources.at(i));
		}
	}
}

void GraphicsManager::resizeIndexBuffer(unsigned int minimumSize)
{
	if (indexBuffer != NULL)
	{
		minimumSize += (minimumSize - indexBufferSize) / 2;
		indexBuffer->Release();
	}
	indexBufferSize = minimumSize;
	printf("resizsing index buffer to fit : %u bytes \n", indexBufferSize);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = indexBufferSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	HRESULT res = dev->CreateBuffer(&bd, NULL, &indexBuffer);
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getType() == RESOURCE_TYPE_MESH)
		{
			writeMesh((Mesh*)graphicsResources.at(i));
		}
	}
}

void GraphicsManager::clearStateBoundResources(State * state)
{
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && graphicsResources.at(i)->getBoundState() == state)
		{
			delete graphicsResources.at(i);
			graphicsResources.at(i) = NULL;
		}
	}
	for (int i = (int)graphicsResources.size() - 1; i >= 0; i--)
	{
		if (graphicsResources.at(i) == NULL)
		{
			graphicsResources.pop_back();
		}
		else
		{
			i = -1;
		}
	}
}

void GraphicsManager::commitNewResources()
{
	for (unsigned int i = 0; i < graphicsResources.size(); i++)
	{
		if (graphicsResources.at(i) != NULL && !graphicsResources.at(i)->isCommited())
		{
			switch (graphicsResources.at(i)->getType())
			{
			case RESOURCE_TYPE_MESH:
				writeMesh((Mesh*)graphicsResources.at(i));
				graphicsResources.at(i)->setCommited(true);
				break;
			case RESOURCE_TYPE_TEXTURE:
				writeTexture((Texture*)graphicsResources.at(i));
				graphicsResources.at(i)->setCommited(true);
				break;
			default:
				break;
			}
		}
	}
}