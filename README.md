# Aurora

### 编译说明

#### 克隆代码

```
git clone https://github.com/xiaoxin0214/Aurora.git --recurse-submodules
```

如果克隆代码时，忘记添加`--recurse-submodules`参数，Aurora依赖的子模块将不会被下载，通过如下命令下载子模块：

```
git submodule update --init --recursive
```

#### 编译代码

Aurora使用premake编译代码

你可以直接双击运行`GenerateProjects.bat`脚本生成`vs2019`解决方案