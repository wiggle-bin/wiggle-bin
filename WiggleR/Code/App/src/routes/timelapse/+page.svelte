<script lang="ts">
	import { start, end, showThresh, showContours } from '../../store/store';
	import { onMount } from 'svelte';

	let plotly: null | any;
	$: plotly = null;
	$: contoursPlot(plotly, startTime, endTime);

	onMount(async () => {
		plotly = await import('plotly.js-dist-min');
	});

	async function contoursPlot(plotly: any, startTime: string, endTime: string) {
		if (!plotly) return;

		const response = await fetch(
			`http://127.0.0.1:8000/contours?startTime=${startTime}&endTime=${endTime}`
		);
		const data = await response.json();

		const trace1: Plotly.Data = {
			x: data.dates,
			y: data.contourArea,
			line: { color: '#fff' },
			type: 'scatter'
		};

		const plotlyData: Plotly.Data[] = [trace1];

		plotly.newPlot('contourAreaPlot', plotlyData, {
			font: {
				color: 'white'
			},
			plot_bgcolor: 'rgb(15, 15, 15)',
			paper_bgcolor: 'rgb(15, 15, 15)'
		});
	}

	// Video
	let startTime = '';
	let endTime = '';
	let threshActive: null | boolean = false;
	let contoursActive: null | boolean = false;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));
	showThresh.subscribe((show) => (threshActive = Boolean(show)));
	showContours.subscribe((show) => (contoursActive = Boolean(show)));
</script>

<svelte:head>
	<script src="https://cdn.plot.ly/plotly-latest.min.js" type="text/javascript"></script>
</svelte:head>

{#key [startTime, endTime, threshActive, contoursActive]}
	<video width="100%" controls>
		<source
			src={`http://localhost:8000/video?startTime=${startTime}&endTime=${endTime}&showThresh=${threshActive}&showContours=${contoursActive}`}
			type="video/mp4"
		/>
	</video>
{/key}

<div id="contourAreaPlot"><!-- Plotly chart will be drawn inside this DIV --></div>
