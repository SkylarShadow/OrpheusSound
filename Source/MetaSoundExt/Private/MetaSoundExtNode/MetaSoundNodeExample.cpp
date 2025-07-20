// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaSoundExtNode/MetaSoundNodeExample.h"
#include "MetasoundExecutableOperator.h"     // TExecutableOperator class
#include "MetasoundPrimitives.h"             // ReadRef and WriteRef descriptions for bool, int32, float, and string
#include "MetasoundNodeRegistrationMacro.h"  // METASOUND_LOCTEXT and METASOUND_REGISTER_NODE macros
#include "MetasoundStandardNodesNames.h"     // StandardNodes namespace
#include "MetasoundFacade.h"				         // FNodeFacade class, eliminates the need for a fair amount of boilerplate code
#include "MetasoundParamHelper.h"    


#define LOCTEXT_NAMESPACE "MetasoundExtNodes_MetasoundExampleNode"

namespace Metasound
{
	//////////////////////////////////////////////////////////////
	//Defining Inputs and Outputs
	namespace ExampleNodeNames
	{
		METASOUND_PARAM(InputAValue, "A", "Input value A.");
		METASOUND_PARAM(InputBValue, "B", "Input value B.");
		METASOUND_PARAM(OutputValue, "Sum of A and B", "The sum of A and B.");

	}


	
	
	class FExampleOperator : public TExecutableOperator<FExampleOperator>
	{

	public:

		//////////////////////////////////////////////////////////////
		//Constructor
		// 
		//The Constructor  will need to create write references to the node’s output parameters, and initialize both the input and output parameters to reasonable values
		// 构造
	
		//edit...
		FExampleOperator(
			const FOperatorSettings& InSettings, const FFloatReadRef& InAValue, const FFloatReadRef& InBValue)
			: InputA(InAValue)
			, InputB(InBValue)
			, ExampleOutput(FFloatWriteRef::CreateNew(*InputA + *InputB))
		{
		}


		//////////////////////////////////////////////////////////////
		// Vertex Interface (DeclareVertexInterface)
		// DeclareVertexInterface function in the above implementation of GetNodeInfo()
		// metasound parameters(pins)are known as vertices, To define a MetaSound node, the graph builder needs to understand the vertices to build the node. 
		// 需要告诉图表构建器要如何构建metasound中的图形节点

		static const FVertexInterface& DeclareVertexInterface()
		{
			using namespace ExampleNodeNames;

			//edit...
			static const FVertexInterface Interface(
				FInputVertexInterface(
					TInputDataVertex<float>
					(METASOUND_GET_PARAM_NAME_AND_METADATA(InputAValue)),

					TInputDataVertex<float>
					(METASOUND_GET_PARAM_NAME_AND_METADATA(InputBValue))
				),
				FOutputVertexInterface(
					 TOutputDataVertex<float>
					(METASOUND_GET_PARAM_NAME_AND_METADATA(OutputValue))
				)
			);

			return Interface;
		}

		//////////////////////////////////////////////////////////////
		// S1:GetNodeInfo
		// This is a function you need to provide for your node which retrieves necessary metadata about the MetaSound node
		// such as node subcategories, localizable display name, a localizable description, the node author, a prompt and so on
		// 节点的一些描述信息封装在这
		static const FNodeClassMetadata& GetNodeInfo()
		{
			auto CreateNodeClassMetadata = []() -> FNodeClassMetadata
				{
					FVertexInterface NodeInterface = DeclareVertexInterface();

					FNodeClassMetadata Metadata //根据这个结构体的成员去填例如 Metadata.ClassName = { TEXT("UE"), TEXT(" ???"), TEXT("Audio") };
					{
							FNodeClassName { StandardNodes::Namespace, "Tutorial Node",
								TEXT("Audio")}, //ControlVariant AudioVariant
							1, // Major Version
							0, // Minor Version
							METASOUND_LOCTEXT("TutorialNodeDisplayName", "Tutorial Node"),
							METASOUND_LOCTEXT("TutorialNodeDesc", "A simple node to demonstrate how to create new MetaSound nodes in C++.Adds two floats together"),
							PluginAuthor,
							PluginNodeMissingPrompt,
							NodeInterface,
							{ },
							{ },
							FNodeDisplayStyle{}
					};

					return Metadata;
				};

			static const FNodeClassMetadata Metadata = CreateNodeClassMetadata();
			return Metadata;
		}


		//////////////////////////////////////////////////////////////
		// S2:GetInputs and S3:GetOutputs
		//This is a virtual function of IOperator (inherited by TExecutableOperator) that you’ll need to implement. 
		//These functions are what allow the MetaSound graph to interact with the inputs and outputs of your nodes, respectively, via returning data reference collections.
		
		virtual FDataReferenceCollection GetInputs() const override
		{
			using namespace ExampleNodeNames;
			FDataReferenceCollection InputDataReferences;

			//edit
			InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputAValue),InputA);
			InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InputBValue),InputB);

			return InputDataReferences;
		}

		virtual FDataReferenceCollection GetOutputs() const override
		{
			using namespace ExampleNodeNames;
			FDataReferenceCollection OutputDataReferences;

			OutputDataReferences.AddDataReadReference(
				METASOUND_GET_PARAM_NAME(OutputValue),ExampleOutput);

			return OutputDataReferences;
		}


		//////////////////////////////////////////////////////////////
		//Stg4:CreateOperator

		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors)
		{
			using namespace ExampleNodeNames;

			const FInputVertexInterfaceData& InputData = InParams.InputData;
			// 有些作者把DeclareVertexInterface放在这里执行？
			//const Metasound::FInputVertexInterface& InputInterface = DeclareVertexInterface().GetInputInterface(); 

			FFloatReadRef InputA = InputData.GetOrCreateDefaultDataReadReference<float>(METASOUND_GET_PARAM_NAME(InputAValue), InParams.OperatorSettings);
			FFloatReadRef InputB = InputData.GetOrCreateDefaultDataReadReference<float>(METASOUND_GET_PARAM_NAME(InputBValue), InParams.OperatorSettings);

			return MakeUnique<FExampleOperator>(InParams.OperatorSettings,InputA, InputB);
		}


		//////////////////////////////////////////////////////////////
		//stg5:Execute

		//MetaSound’s primary functionality goes,this is where you should place any math, digital signal processing, or other operations that you want the node to perform each frame.
		// input ->system -> output
		// 按signal system 来理解，就是系统内部的逻辑
		void Execute()
		{
			*ExampleOutput = *InputA + *InputB;
		}


		//////////////////////////////////////////////////////////////
	// Data References
	// declare input output ref
	private:

		// Inputs
		// 声明输入
		FFloatReadRef InputA;
		FFloatReadRef InputB;

		// Outputs
		// 声明输出
		FFloatWriteRef ExampleOutput;


	};
	
	//// Node Class - Inheriting from FNodeFacade is recommended for nodes that have a static FVertexInterface
	class FExampleNode : public FNodeFacade
	{
	public:
		FExampleNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FExampleOperator>())
		{
		}
	};

	// Register node
	METASOUND_REGISTER_NODE(FExampleNode);
}


#undef LOCTEXT_NAMESPACE